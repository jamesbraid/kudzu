#!/usr/bin/env python3
"""Write the windows, bases and gp_literals sections of scripts/inventory-config.json from scripts/hw-windows.json.

Usage: scripts/gen-hw-windows.py [--check] [--chips PATH]

hw-windows.json says which register windows the inventory scan looks for in the kernels and how code
reaches them. The addresses, pointer arguments and pointer-returning routines that the system-chip
workspace (chips) already established are not copied: this script reads that workspace's
tools/configs/<chip>.json at the commit pinned in hw-windows.json, with `git show`, so a window cannot
drift from the census that found it. The checkout is given with --chips or found at the path the file
names (a clone at .cache/siblings/chips); it must contain the pinned commit. With --check nothing is
written and the run fails when the config is out of date. Run it like the other scripts, from the
repository root, then run retools/tools/inventory.py again.
"""
import json
import os
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CFG = os.path.join(REPO, 'scripts', 'inventory-config.json')
SRC = os.path.join(REPO, 'scripts', 'hw-windows.json')


def num(x):
    return int(x, 0) if isinstance(x, str) else int(x)


def hx(v):
    return '0x%x' % v


class Chips:
    def __init__(self, path, pin):
        self.path, self.pin, self.cache = path, pin, {}
        r = subprocess.run(['git', '-C', path, 'cat-file', '-e', pin + '^{commit}'], capture_output=True)
        if r.returncode:
            sys.exit('gen-hw-windows: %s does not contain chips commit %s (fetch it, or pass --chips)' % (path, pin))

    def config(self, chip):
        if chip not in self.cache:
            r = subprocess.run(['git', '-C', self.path, 'show', '%s:tools/configs/%s.json' % (self.pin, chip)],
                               capture_output=True, text=True)
            if r.returncode:
                sys.exit('gen-hw-windows: chips %s has no tools/configs/%s.json: %s' % (self.pin[:12], chip, r.stderr.strip()))
            self.cache[chip] = json.loads(r.stdout)
        return self.cache[chip]

    def const_windows(self, chip, name, image=None):
        """Sorted distinct (lo, hi) of the chip config's constant windows with that name."""
        out = set()
        for im in self.config(chip)['images']:
            if image and im['label'] != image:
                continue
            for w in im.get('const_windows') or []:
                if w['name'] == name:
                    out.add((num(w['lo']), num(w['hi'])))
        if not out:
            sys.exit('gen-hw-windows: chips %s has no constant window %s%s' % (chip, name, ' in ' + image if image else ''))
        return sorted(out)


def main():
    src = json.load(open(SRC))
    chips_path = os.path.join(REPO, src['chips']['checkout'])
    if '--chips' in sys.argv:
        chips_path = sys.argv[sys.argv.index('--chips') + 1]
    chips = Chips(chips_path, src['chips']['pin'])
    pin = src['chips']['pin'][:12]
    plat = src['platforms']

    def cite(chip, why):
        return '%s [chips %s, tools/configs/%s.json]' % (why, pin, chip)

    windows = []
    names = set()
    for w in src['windows']:
        w = dict(w)
        names.add(w['name'])
        windows.append(w)
    bases, seen = [], set()

    def check_window(e):
        if e['window'] not in names:
            sys.exit('gen-hw-windows: %s is not a window in hw-windows.json' % e['window'])

    def add(b):
        key = json.dumps(b, sort_keys=True)
        if key not in seen:
            seen.add(key)
            bases.append(b)

    for e in src['constants']:
        check_window(e)
        found = chips.const_windows(e['chip'], e['name'], e.get('image'))
        ranges = []
        if 'lo' in e:
            lo, hi = num(e['lo']), num(e['hi'])
            if not any(a <= lo and hi <= b for a, b in found):
                sys.exit('gen-hw-windows: %s %s is not inside chips window %s %s' %
                         (e['window'], e['label'], e['name'], [(hx(a), hx(b)) for a, b in found]))
            ranges = [(lo, hi)]
        else:
            ranges = found
        for lo, hi in ranges:
            b = {'type': 'constant', 'lo': hx(lo), 'hi': hx(hi), 'window': e['window'], 'translate': False}
            if e.get('computed'):
                b['computed'] = True
            b['label'] = e['label']
            b['why'] = cite(e['chip'], e['why'])
            b['binaries'] = plat[e['platform']]
            add(b)

    for e in src['layout_constants']:
        check_window(e)
        b = {'type': 'constant', 'lo': e['lo'], 'hi': e['hi'], 'window': e['window'], 'translate': False,
             'label': e['label'], 'why': e['why'], 'binaries': plat[e['platform']]}
        add(b)

    for e in src['arguments']:
        check_window(e)
        s = chips.config(e['chip'])['seeds']['args'][e['seed']]
        if num(s['off']) != 0:
            sys.exit('gen-hw-windows: chips %s argument seed %d has offset %s, not a register pointer' %
                     (e['chip'], e['seed'], s['off']))
        add({'type': 'argument', 'function': s['functions'], 'reg': s['reg'], 'window': e['window'], 'offset': 0,
             'label': e['label'], 'why': cite(e['chip'], e['why']), 'binaries': plat[e['platform']]})

    for e in src['returns']:
        check_window(e)
        if 'chip' in e:
            hit = [r for r in chips.config(e['chip'])['seeds']['returns']
                   if r['callee'] == e['callee'] and 'arg' not in r and num(r['off']) == 0]
            if not hit:
                sys.exit('gen-hw-windows: chips %s has no plain return seed for %s' % (e['chip'], e['callee']))
            callees, why = [e['callee']], cite(e['chip'], e['why'])
        else:
            callees, why = e['callees'], e['why']
        for callee in callees:
            add({'type': 'return', 'function': callee, 'window': e['window'],
                 'offset': None if e.get('offset', 0) is None else num(e.get('offset', 0)),
                 'label': e['label'],
                 'why': why, 'binaries': plat[e['platform']]})

    cfg = json.load(open(CFG))
    cfg['windows'], cfg['bases'] = windows, bases
    cfg['gp_literals'] = [{k: v for k, v in r.items() if k != 'why'} for r in src['gp_literals']]
    text = json.dumps(cfg, indent=2) + '\n'
    if '--check' in sys.argv[1:]:
        if open(CFG).read() != text:
            sys.exit('scripts/inventory-config.json is out of date: run scripts/gen-hw-windows.py')
        print('config matches hw-windows.json')
        return
    open(CFG, 'w').write(text)
    print('%d windows, %d bases' % (len(windows), len(bases)))


main()
