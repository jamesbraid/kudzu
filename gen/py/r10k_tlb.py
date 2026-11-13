"""generated from defs/r10k_tlb.toml by defs-gen - do not edit, change the source
The wired-entry layout IRIX uses in the R10000 TLB"""

UNIT = 'r10k_tlb'
REGISTERS = {}
WORDS = {}
FIELDS = {}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'ASID_USABLE': {'value': 255, 'count': 1, 'stride': 1}, 'BASE_PAGE_BYTES': {'value': 16384, 'count': 1, 'stride': 1}, 'BROADCAST_SHARERS': {'value': 16, 'count': 1, 'stride': 1}, 'IO_MAP_BYTES': {'value': 16777216, 'count': 1, 'stride': 1}, 'KTEXT_BYTES': {'value': 16777216, 'count': 1, 'stride': 1}, 'LARGE_PAGE_MIN_BYTES': {'value': 65536, 'count': 1, 'stride': 1}, 'RECYCLE_TICKS_DEFAULT': {'value': 100, 'count': 1, 'stride': 1}, 'WIRED_INDEX_CPU': {'value': 0, 'count': 1, 'stride': 1}, 'WIRED_INDEX_DROPIN_FIRST': {'value': 2, 'count': 1, 'stride': 1}, 'WIRED_INDEX_DROPIN_LAST': {'value': 6, 'count': 1, 'stride': 1}, 'WIRED_INDEX_KTEXT': {'value': 1, 'count': 1, 'stride': 1}, 'WIRED_MAPPED': {'value': 7, 'count': 1, 'stride': 1}, 'WIRED_UNMAPPED': {'value': 6, 'count': 1, 'stride': 1}}
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
