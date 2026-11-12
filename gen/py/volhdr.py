"""generated from defs/volhdr.toml by defs-gen - do not edit, change the source
SGI disk volume header"""

UNIT = 'volhdr'
REGISTERS = {}
WORDS = {'DIRENT_W2': {'width': 32, 'grade': 'established'}, 'DIRENT_W3': {'width': 32, 'grade': 'established'}, 'HDR_W0': {'width': 32, 'grade': 'established'}, 'HDR_W1': {'width': 32, 'grade': 'established'}, 'HDR_W10': {'width': 32, 'grade': 'established'}, 'HDR_W11': {'width': 32, 'grade': 'inferred'}, 'HDR_W126': {'width': 32, 'grade': 'established'}, 'HDR_W17': {'width': 32, 'grade': 'established'}, 'HDR_W7': {'width': 32, 'grade': 'inferred'}, 'HDR_W8': {'width': 32, 'grade': 'established'}, 'HDR_W9': {'width': 32, 'grade': 'inferred'}, 'PART_W0': {'width': 32, 'grade': 'established'}, 'PART_W1': {'width': 32, 'grade': 'established'}, 'PART_W2': {'width': 32, 'grade': 'established'}}
FIELDS = {'DIRENT_W2': {'start_block': (31, 0)}, 'DIRENT_W3': {'length_bytes': (31, 0)}, 'HDR_W0': {'magic': (31, 0)}, 'HDR_W1': {'root': (31, 16), 'swap': (15, 0)}, 'HDR_W10': {'dp_secbytes': (31, 16)}, 'HDR_W11': {'dp_flags': (31, 0)}, 'HDR_W126': {'checksum': (31, 0)}, 'HDR_W17': {'dp_capacity': (31, 0)}, 'HDR_W7': {'dp_cylinders': (31, 16), 'dp_head_offset': (15, 0)}, 'HDR_W8': {'dp_heads': (31, 16), 'dp_tag_depth': (15, 8), 'dp_cyl_high': (7, 0)}, 'HDR_W9': {'dp_sectors': (15, 0)}, 'PART_W0': {'nblks': (31, 0)}, 'PART_W1': {'first': (31, 0)}, 'PART_W2': {'type': (31, 0)}}
ENUMS = {'PART_W2.type': {0: 'volhdr', 1: 'trkrepl', 2: 'secrepl', 3: 'raw', 4: 'bsd', 5: 'sysv', 6: 'volume', 7: 'efs', 8: 'lvol', 9: 'rlvol', 10: 'xfs', 11: 'xfslog', 12: 'xlv', 13: 'xvm', 14: 'vxvm'}}
FIELD_ENUM = {'PART_W2.type': 'PART_W2.type'}
CONSTANTS = {'BOOTFILE_OFF': {'value': 8, 'count': 1, 'stride': 1}, 'CSUM_OFF': {'value': 504, 'count': 1, 'stride': 1}, 'DIRENT_LEN_NAME': {'value': 8, 'count': 1, 'stride': 1}, 'DIRENT_OFF_NAME': {'value': 0, 'count': 1, 'stride': 1}, 'DIRENT_SIZE': {'value': 16, 'count': 1, 'stride': 1}, 'DIR_COUNT': {'value': 15, 'count': 1, 'stride': 1}, 'DIR_NAME_LEN': {'value': 8, 'count': 1, 'stride': 1}, 'DIR_OFF': {'value': 72, 'count': 1, 'stride': 1}, 'DIR_STRIDE': {'value': 16, 'count': 1, 'stride': 1}, 'DP_BYTES': {'value': 48, 'count': 1, 'stride': 1}, 'DP_FLAG_TAGGED': {'value': 64, 'count': 1, 'stride': 1}, 'DP_OFF': {'value': 24, 'count': 1, 'stride': 1}, 'HDR_LEN_BOOTFILE': {'value': 16, 'count': 1, 'stride': 1}, 'HDR_OFF_BOOTFILE': {'value': 8, 'count': 1, 'stride': 1}, 'HDR_SIZE': {'value': 512, 'count': 1, 'stride': 1}, 'MAGIC': {'value': 199600449, 'count': 1, 'stride': 1}, 'PART_COUNT': {'value': 16, 'count': 1, 'stride': 1}, 'PART_OFF': {'value': 312, 'count': 1, 'stride': 1}, 'PART_SIZE': {'value': 12, 'count': 1, 'stride': 1}, 'PART_STRIDE': {'value': 12, 'count': 1, 'stride': 1}, 'SIZE': {'value': 512, 'count': 1, 'stride': 1}, 'SLOT_ROOT': {'value': 0, 'count': 1, 'stride': 1}, 'SLOT_SWAP': {'value': 1, 'count': 1, 'stride': 1}, 'SLOT_VOLHDR': {'value': 8, 'count': 1, 'stride': 1}, 'SLOT_WHOLEDISK': {'value': 10, 'count': 1, 'stride': 1}}
TESTS = [('DIRENT_W3', 267264, {'length_bytes': 267264}), ('PART_W0', 4096, {'nblks': 4096}), ('PART_W0', 262144, {'nblks': 262144}), ('PART_W1', 266240, {'first': 266240})]


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
