"""generated from defs/efs.toml by defs-gen - do not edit, change the source
Extent File System on-disk structures"""

UNIT = 'efs'
REGISTERS = {}
WORDS = {'DB_W0': {'width': 32, 'grade': 'established'}, 'DEV_W8': {'width': 32, 'grade': 'established'}, 'DEV_W9': {'width': 32, 'grade': 'established'}, 'DI_W0': {'width': 32, 'grade': 'established'}, 'DI_W1': {'width': 32, 'grade': 'established'}, 'DI_W2': {'width': 32, 'grade': 'established'}, 'DI_W3': {'width': 32, 'grade': 'established'}, 'DI_W4': {'width': 32, 'grade': 'established'}, 'DI_W5': {'width': 32, 'grade': 'established'}, 'DI_W6': {'width': 32, 'grade': 'established'}, 'DI_W7': {'width': 32, 'grade': 'established'}, 'EXT_W0': {'width': 32, 'grade': 'established'}, 'EXT_W1': {'width': 32, 'grade': 'established'}, 'SB_W0': {'width': 32, 'grade': 'established'}, 'SB_W1': {'width': 32, 'grade': 'established'}, 'SB_W11': {'width': 32, 'grade': 'established'}, 'SB_W12': {'width': 32, 'grade': 'established'}, 'SB_W13': {'width': 32, 'grade': 'established'}, 'SB_W14': {'width': 32, 'grade': 'established'}, 'SB_W15': {'width': 32, 'grade': 'established'}, 'SB_W16': {'width': 32, 'grade': 'inferred'}, 'SB_W2': {'width': 32, 'grade': 'established'}, 'SB_W22': {'width': 32, 'grade': 'established'}, 'SB_W3': {'width': 32, 'grade': 'established'}, 'SB_W4': {'width': 32, 'grade': 'established'}, 'SB_W5': {'width': 32, 'grade': 'established'}, 'SB_W6': {'width': 32, 'grade': 'established'}, 'SB_W7': {'width': 32, 'grade': 'established'}}
FIELDS = {'DB_W0': {'magic': (31, 16), 'firstused': (15, 8), 'slots': (7, 0)}, 'DEV_W8': {'odev': (31, 16)}, 'DEV_W9': {'ndev': (31, 0)}, 'DI_W0': {'mode_type': (31, 28), 'mode_perm': (27, 16), 'nlink': (15, 0)}, 'DI_W1': {'uid': (31, 16), 'gid': (15, 0)}, 'DI_W2': {'size': (31, 0)}, 'DI_W3': {'atime': (31, 0)}, 'DI_W4': {'mtime': (31, 0)}, 'DI_W5': {'ctime': (31, 0)}, 'DI_W6': {'gen': (31, 0)}, 'DI_W7': {'numextents': (31, 16), 'version': (15, 8), 'spare': (7, 0)}, 'EXT_W0': {'magic': (31, 24), 'bn': (23, 0)}, 'EXT_W1': {'length': (31, 24), 'offset': (23, 0)}, 'SB_W0': {'size': (31, 0)}, 'SB_W1': {'firstcg': (31, 0)}, 'SB_W11': {'bmsize': (31, 0)}, 'SB_W12': {'tfree': (31, 0)}, 'SB_W13': {'tinode': (31, 0)}, 'SB_W14': {'bmblock': (31, 0)}, 'SB_W15': {'replsb': (31, 0)}, 'SB_W16': {'lastialloc': (31, 0)}, 'SB_W2': {'cgfsize': (31, 0)}, 'SB_W22': {'checksum': (31, 0)}, 'SB_W3': {'cgisize': (31, 16), 'sectors': (15, 0)}, 'SB_W4': {'heads': (31, 16), 'ncg': (15, 0)}, 'SB_W5': {'dirty': (31, 16)}, 'SB_W6': {'time': (31, 0)}, 'SB_W7': {'magic': (31, 0)}}
ENUMS = {'DI_W0.mode_type': {1: 'fifo', 2: 'chr', 3: 'x3', 4: 'dir', 6: 'blk', 7: 'x7', 8: 'reg', 10: 'lnk', 12: 'sock'}, 'SB_W5.dirty': {0: 'clean', 2989: 'dirty', 4660: 'needsck', 30583: 'mounted'}, 'SB_W7.magic': {469337: 'old', 469338: 'new'}}
FIELD_ENUM = {'DI_W0.mode_type': 'DI_W0.mode_type', 'SB_W5.dirty': 'SB_W5.dirty', 'SB_W7.magic': 'SB_W7.magic'}
CONSTANTS = {'BBSIZE': {'value': 512, 'count': 1, 'stride': 1}, 'BITMAP_BB': {'value': 2, 'count': 1, 'stride': 1}, 'DB_SIZE': {'value': 512, 'count': 1, 'stride': 1}, 'DENT_HEADER_BYTES': {'value': 5, 'count': 1, 'stride': 1}, 'DEV_LONG_FORM': {'value': 65535, 'count': 1, 'stride': 1}, 'DIRBLK_HEADERSIZE': {'value': 4, 'count': 1, 'stride': 1}, 'DIRBLK_MAGIC': {'value': 48879, 'count': 1, 'stride': 1}, 'DIRBLK_MAXSLOTS': {'value': 72, 'count': 1, 'stride': 1}, 'DIRECTEXTENTS': {'value': 12, 'count': 1, 'stride': 1}, 'DI_SIZE': {'value': 128, 'count': 1, 'stride': 1}, 'EXT_SIZE': {'value': 8, 'count': 1, 'stride': 1}, 'EX_MAXLEN': {'value': 248, 'count': 1, 'stride': 1}, 'INODES_PER_BB': {'value': 4, 'count': 1, 'stride': 1}, 'INODE_DATA_BYTES': {'value': 96, 'count': 1, 'stride': 1}, 'INODE_SIZE': {'value': 128, 'count': 1, 'stride': 1}, 'MAGIC_NEW': {'value': 469338, 'count': 1, 'stride': 1}, 'MAGIC_OLD': {'value': 469337, 'count': 1, 'stride': 1}, 'ROOT_INO': {'value': 2, 'count': 1, 'stride': 1}, 'SB_LEN_FNAME': {'value': 6, 'count': 1, 'stride': 1}, 'SB_LEN_FPACK': {'value': 6, 'count': 1, 'stride': 1}, 'SB_LEN_SPARE': {'value': 20, 'count': 1, 'stride': 1}, 'SB_OFF_FNAME': {'value': 32, 'count': 1, 'stride': 1}, 'SB_OFF_FPACK': {'value': 38, 'count': 1, 'stride': 1}, 'SB_OFF_SPARE': {'value': 68, 'count': 1, 'stride': 1}, 'SB_SIZE': {'value': 92, 'count': 1, 'stride': 1}, 'SUPER_BB': {'value': 1, 'count': 1, 'stride': 1}}
TESTS = []


def decode(name, value):
    """Field name -> value for a register or word."""
    return {f: (value >> lo) & ((1 << (hi - lo + 1)) - 1) for f, (hi, lo) in FIELDS[name].items()}


def encode(name, **fields):
    v = 0
    for f, x in fields.items():
        hi, lo = FIELDS[name][f]
        if x >> (hi - lo + 1):
            raise ValueError('%s.%s: %d does not fit' % (name, f, x))
        v |= x << lo
    return v


def describe(name, value):
    """decode, with enum labels where the field has an enum."""
    out = {}
    for f, x in decode(name, value).items():
        e = ENUMS.get(FIELD_ENUM.get('%s.%s' % (name, f)))
        out[f] = e.get(x, x) if e else x
    return out


def selftest():
    for name, value, expect in TESTS:
        got = decode(name, value)
        for f, want in expect.items():
            assert got[f] == want, (name, hex(value), f, got[f], want)
    return len(TESTS)


if __name__ == '__main__':
    print('%s: %d vectors ok' % (UNIT, selftest()))
