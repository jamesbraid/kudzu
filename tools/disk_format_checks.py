"""Cross-check the disk-format decode headers against the layout tables in formats/<name>/README.md.

The decode grammar cuts every structure into aligned 32-bit big-endian words, so
a field's byte position is only implicit in its word number and bit range. The
prose notes state positions as byte offset and size. This plugin reads the
"offset | size | `name`" table rows of each configured section and checks that
the header agrees:

  - a scalar row has a field of that name in the word at offset / 4, at the bit
    range its offset and size imply;
  - a row that the header splits (bit flags, a 64-bit value in two words, an
    enumerated byte pair) has fields whose names start with the row name and
    which together cover every byte of the row and nothing outside it;
  - a byte-string row has `<PREFIX>_<STRUCT>_OFF_<NAME>` and `_LEN_<NAME>`
    constants equal to its offset and size.

A row that matches nothing in the header is a failure, unless it is listed in
SKIP below. That catches a field added to the notes but not to the header.
"""
import re

ROW = re.compile(
    r'^\|\s*(0x[0-9a-fA-F]+|\d+)\s*\|\s*(?:up to )?(0x[0-9a-fA-F]+|\d+)(?:\+(\d+))?\s*\|\s*`(\w+)`')
UNION_ROW = re.compile(
    r'^\|\s*(slice|stripe|mirror|raid|copy-on-write|subvolume|volume)\s*\|\s*(\d+)\s*\|\s*(\d+)\s*\|\s*`(\w+)`')
UNION_KEY = {'slice': 'VE_SLICE', 'stripe': 'VE_STRIPE', 'mirror': 'VE_MIRROR', 'raid': 'VE_RAID',
             'copy-on-write': 'VE_COW', 'subvolume': 'VE_SUBVOL', 'volume': 'VE_VOL'}

SECTIONS = {
    'xvm': [
        dict(doc='formats/xvm/README.md', heading=r'Physical label \(`xvm_physlab`', key='PL', strip='pl_'),
        dict(doc='formats/xvm/README.md', heading=r'Label sector header \(`xvm_labelhdr`', key='LH', strip='lh_'),
        dict(doc='formats/xvm/README.md', heading=r'Virtual-element entry \(`xvm_disk_ve`', key='VE', strip='dve_', union=True),
        dict(doc='formats/xvm/README.md', heading=r'Attribute entry \(`xvm_disk_attr`', key='ATTR', strip='da_'),
    ],
    'xlv': [
        dict(doc='formats/xlv/README.md', heading=r'Master block', key='MASTER', strip=''),
        dict(doc='formats/xlv/README.md', heading=r'Header \(`0x120` bytes\)', key='LBL', strip=''),
        dict(doc='formats/xlv/README.md', heading=r'Entry \(`0x118` bytes\)', key='E', strip=''),
    ],
    'efs': [
        dict(doc='formats/efs/README.md', heading=r'Superblock \(block 1', key='SB', strip='fs_'),
        dict(doc='formats/efs/README.md', heading=r'Inode \(128 bytes\)', key='DI', strip='di_'),
    ],
    'xfs': [
        dict(doc='formats/xfs/README.md', heading=r'1\.2 Field table', key='SB', strip=''),
        dict(doc='formats/xfs/README.md', heading=r'2\.1 Core', key='INO', strip=''),
    ],
}
# Rows the header deliberately does not describe: the label's raw trailing
# bytes, and the inode data area, which is a union of forks.
SKIP = {'u1', 'u', 'rest'}


def section_rows(text, heading):
    lines = text.splitlines()
    start = None
    for i, line in enumerate(lines):
        if line.startswith('#') and re.search(heading, line):
            start = i + 1
            break
    if start is None:
        return None
    rows = []
    for line in lines[start:]:
        if line.startswith('#'):
            break
        rows.append(line)
    return rows


def index_fields(hdr):
    """key -> list of (word, name, hi, lo) for the fields of every structure."""
    out = {}
    for fkey, (hi, lo, _g, _n, _owner) in hdr.fields.items():
        word, name = fkey.split('.', 1)
        m = re.match(r'^(.*)_W(\d+)$', word)
        if m:
            out.setdefault(m.group(1), []).append((int(m.group(2)), name, hi, lo))
    return out


def byte_span(word, hi, lo):
    """First and last byte (inclusive, from the start of the structure) a field touches."""
    return 4 * word + (31 - hi) // 8, 4 * word + (31 - lo) // 8


def check_row(ctx, hdr, where, idx, key, name, off, size):
    prefix = hdr.cfg['prefix']
    arr_off = hdr.defs.get('%s_%s_OFF_%s' % (prefix, key, name.upper()))
    if arr_off is not None:
        arr_len = hdr.defs.get('%s_%s_LEN_%s' % (prefix, key, name.upper()))
        if (arr_off, arr_len) != (off, size):
            ctx.rep.fail(where, '%s.%s: notes say offset %d size %d, header constants say %s and %s'
                         % (key, name, off, size, arr_off, arr_len))
        return
    fields = idx.get(key, [])
    exact = [f for f in fields if f[1] == name]
    if exact and size in (1, 2, 4):
        word, _n, hi, lo = exact[0]
        want_hi = 31 - 8 * (off % 4)
        want_lo = want_hi - 8 * size + 1
        if (word, hi, lo) != (off // 4, want_hi, want_lo):
            ctx.rep.fail(where, '%s.%s: notes say offset %d size %d (word %d bits %d:%d), header has word %d bits %d:%d'
                         % (key, name, off, size, off // 4, want_hi, want_lo, word, hi, lo))
        return
    parts = [f for f in fields if f[1] == name or f[1].startswith(name + '_')]
    if not parts:
        if name not in SKIP:
            ctx.rep.fail(where, '%s: notes have a field "%s" (offset %d, size %d) that the header does not describe'
                         % (key, name, off, size))
        return
    covered = set()
    for word, fname, hi, lo in parts:
        first, last = byte_span(word, hi, lo)
        if first < off or last > off + size - 1:
            ctx.rep.fail(where, '%s.%s lies outside bytes %d to %d given by the notes for "%s"'
                         % (key, fname, off, off + size - 1, name))
        covered.update(range(first, last + 1))
    missing = sorted(set(range(off, off + size)) - covered)
    if missing:
        ctx.rep.fail(where, '%s: the header fields for "%s" leave bytes %s uncovered'
                     % (key, name, ', '.join(str(b) for b in missing)))


def check_header(ctx, hdr):
    sections = SECTIONS.get(hdr.unit.name)
    if not sections:
        return
    idx = index_fields(hdr)
    for sec in sections:
        text = ctx.read_doc(sec['doc'])
        rows = section_rows(text, sec['heading'])
        where = '%s vs %s' % (hdr.file, sec['doc'])
        if rows is None:
            ctx.rep.fail(where, 'section %r not found in the notes' % sec['heading'])
            continue
        seen = 0
        for line in rows:
            m = ROW.match(line)
            if m:
                off, size = int(m.group(1), 0), int(m.group(2), 0)
                if m.group(3):
                    size += int(m.group(3))
                raw = m.group(4)
                name = raw[len(sec['strip']):] if sec['strip'] and raw.startswith(sec['strip']) else raw
                check_row(ctx, hdr, where, idx, sec['key'], name, off, size)
                seen += 1
                continue
            if sec.get('union'):
                u = UNION_ROW.match(line)
                if u:
                    key = UNION_KEY[u.group(1)]
                    raw = u.group(4)
                    name = re.sub(r'^v[a-z]{1,3}_', '', raw)
                    check_row(ctx, hdr, where, idx, key, name, 56 + int(u.group(2)), int(u.group(3)))
                    seen += 1
        if not seen:
            ctx.rep.fail(where, 'no table rows found under %r' % sec['heading'])
