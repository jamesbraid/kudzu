"""generated from defs/hubmdb.toml by defs-gen - do not edit, change the source
Origin 300 hub memory-directory migration registers"""

UNIT = 'hubmdb'
REGISTERS = {'mig_diff_thresh': {'offset': 64, 'access': 'w', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}}
WORDS = {}
FIELDS = {'mig_diff_thresh': {'value': (14, 0)}}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'MIG_CANDIDATE_CLR_OFFSET': {'value': 56, 'count': 1, 'stride': 1}, 'MIG_CANDIDATE_OFFSET': {'value': 48, 'count': 1, 'stride': 1}, 'MIG_CONFIG_LOCAL_OFFSET': {'value': 7864344, 'count': 1, 'stride': 1}, 'MIG_DIFF_THRESH_VALID_BIT': {'value': 63, 'count': 1, 'stride': 1}, 'MIG_VALUE_THRESH_OFFSET': {'value': 72, 'count': 1, 'stride': 1}, 'NODE_SHIFT': {'value': 33, 'count': 1, 'stride': 1}, 'WINDOW_BASE_UNITS': {'value': 508, 'count': 1, 'stride': 1}, 'WINDOW_BASE_UNIT_SHIFT': {'value': 16, 'count': 1, 'stride': 1}}
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
