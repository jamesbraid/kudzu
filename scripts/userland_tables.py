#!/usr/bin/env python3
"""Base user-space rows: docs/inventory/userland.tsv, one row per binary in scripts/userland-roles.json.

Usage: scripts/userland_tables.py [--release 6.5.30] [--out docs/inventory]

Run from the repository root with root/ populated, inside the toolchain container
(tools/re-run python3 scripts/userland_tables.py); the readers come from that container.

Input: root/<release>/<install path> for every key of scripts/userland-roles.json. The
ELF facts (class, type, ABI from the header flags, interpreter, section headers and the
count of undefined dynamic symbols) are read from the file itself. The package, type,
interfaces, role, grade and citation come from the roles file and nowhere else.

Output: userland.tsv, tab-separated and deterministic. A value that cannot be read from the
file is written as "unreadable", never guessed.
"""
import argparse
import hashlib
import json
import os
import re
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
READELF = os.environ.get('READELF', 'mips64-linux-gnuabi64-readelf')
ROLES = os.path.join(REPO, 'scripts', 'userland-roles.json')

EF_MIPS_ABI2 = 0x00000020       # N32
EF_MIPS_ABI_O32 = 0x00001000    # O32

HEADER = ['binary', 'package', 'type', 'abi', 'elf_type', 'bytes', 'sha256', 'interpreter',
          'section_headers', 'dyn_undefined', 'status', 'grade', 'role', 'interfaces',
          'citation', 'limits']


def run(args):
    return subprocess.run([READELF] + args, capture_output=True, text=True)


def failed(result):
    # readelf reports a truncated file on stderr and still exits 0, so the message is checked.
    return any(l.startswith('readelf: Error') for l in (result.stdout + result.stderr).splitlines())


def elf_facts(path):
    head = run(['-h', '-W', path])
    if head.returncode != 0:
        return {'abi': 'unreadable', 'elf_type': 'unreadable', 'interpreter': 'unreadable',
                'section_headers': 'unreadable', 'dyn_undefined': 'unreadable'}
    text = head.stdout

    def field(name):
        m = re.search(r'^\s*%s:\s*(.+)$' % name, text, re.M)
        return m.group(1).strip() if m else ''

    cls = field('Class')
    flag_text = field('Flags').split(',')[0].split()
    flags = int(flag_text[0], 16) if flag_text else 0
    if cls == 'ELF64':
        abi = 'n64'
    elif flags & EF_MIPS_ABI2:
        abi = 'n32'
    elif flags & EF_MIPS_ABI_O32:
        abi = 'o32'
    else:
        abi = 'unknown'
    elf_type = field('Type').split(' ')[0].lower()

    prog = run(['-l', '-W', path])
    m = re.search(r'Requesting program interpreter: (\S+?)\]', prog.stdout)
    interp = m.group(1) if m else '-'

    sec = run(['-S', '-W', path])
    if sec.returncode == 0 and not failed(sec):
        count = len([l for l in sec.stdout.splitlines() if re.match(r'\s+\[\s*\d+\]', l)]) - 1
        section_headers = str(count)
    else:
        section_headers = 'unreadable'

    dyn = run(['--dyn-syms', '-W', path])
    if dyn.returncode == 0 and not failed(dyn):
        names = set()
        for line in dyn.stdout.splitlines():
            # An optional import carries an extra [OPTIONAL] flag word before the section index, so the
            # index and name are taken from the end of the line, not from fixed columns.
            parts = line.split()
            if len(parts) >= 8 and parts[-2] == 'UND' and re.match(r'\d+:$', parts[0]):
                names.add(parts[-1].split('@')[0])
        dyn_undefined = str(len(names))
    else:
        dyn_undefined = 'unreadable'

    return {'abi': abi, 'elf_type': elf_type, 'interpreter': interp,
            'section_headers': section_headers, 'dyn_undefined': dyn_undefined}


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1 << 20), b''):
            h.update(chunk)
    return h.hexdigest()


def clean(value):
    return str(value).replace('\t', ' ').replace('\n', ' ').strip()


def main(argv):
    ap = argparse.ArgumentParser()
    ap.add_argument('--release', default='6.5.30')
    ap.add_argument('--out', default=os.path.join(REPO, 'docs', 'inventory'))
    opts = ap.parse_args(argv)

    with open(ROLES) as f:
        roles = json.load(f)
    entries = {k: v for k, v in roles.items() if not k.startswith('_')}
    base = os.path.join(REPO, 'root', opts.release)

    rows = []
    missing = []
    for rel in sorted(entries):
        path = os.path.join(base, rel)
        if not os.path.isfile(path):
            missing.append(rel)
            continue
        entry = entries[rel]
        facts = elf_facts(path)
        rows.append([
            '%s/%s' % (opts.release, rel), clean(entry.get('package', '')),
            clean(entry.get('type', '')), facts['abi'], facts['elf_type'],
            str(os.path.getsize(path)), sha256(path), facts['interpreter'],
            facts['section_headers'], facts['dyn_undefined'],
            clean(entry.get('status', 'located')), clean(entry.get('grade', 'unknown')),
            clean(entry.get('role', '')), clean(entry.get('interfaces', '')),
            clean(entry.get('citation', '')), clean(entry.get('limits', '')),
        ])
    if missing:
        sys.stderr.write('missing from root/%s: %s\n' % (opts.release, ', '.join(missing)))
        return 1

    out = os.path.join(opts.out, 'userland.tsv')
    with open(out, 'w') as f:
        f.write('\t'.join(HEADER) + '\n')
        for row in rows:
            f.write('\t'.join(row) + '\n')
    print('%d rows to %s' % (len(rows), os.path.relpath(out, REPO)))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
