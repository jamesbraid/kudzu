"""generated from defs/hubni.toml by defs-gen - do not edit, change the source
node hub network interface registers"""

UNIT = 'hubni'
REGISTERS = {}
WORDS = {'NSRI': {'width': 32, 'grade': 'established'}}
FIELDS = {'NSRI': {'nodeid': (16, 8)}}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'O200_STATUS_OFFSET': {'value': 6291456, 'count': 1, 'stride': 1}, 'O300_ERROR_CLEAR_OFFSET': {'value': 6816032, 'count': 1, 'stride': 1}, 'O300_ERROR_LATCHED_HI': {'value': 52, 'count': 1, 'stride': 1}, 'O300_ERROR_LATCHED_LO': {'value': 24, 'count': 1, 'stride': 1}, 'O300_ERROR_LINK_RESET_BIT': {'value': 52, 'count': 1, 'stride': 1}, 'O300_ERROR_OFFSET': {'value': 6816000, 'count': 1, 'stride': 1}, 'O300_ERROR_REPORT_A_OFFSET': {'value': 6816008, 'count': 1, 'stride': 1}, 'O300_ERROR_REPORT_B_OFFSET': {'value': 6816016, 'count': 1, 'stride': 1}, 'O300_ERROR_REPORT_C_OFFSET': {'value': 6816024, 'count': 1, 'stride': 1}, 'O300_PORT_RESET_OFFSET': {'value': 6815752, 'count': 1, 'stride': 1}, 'O300_RESET_ENABLE_OFFSET': {'value': 6815760, 'count': 1, 'stride': 1}, 'O300_STATUS_OFFSET': {'value': 6815744, 'count': 1, 'stride': 1}}
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
