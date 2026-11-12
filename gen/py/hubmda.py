"""generated from defs/hubmda.toml by defs-gen - do not edit, change the source
Origin 200/2000 hub memory-directory migration registers"""

UNIT = 'hubmda'
REGISTERS = {'mig_diff_thresh': {'offset': 48, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'mig_value_thresh': {'offset': 56, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'mig_candidate': {'offset': 72, 'access': 'r', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}}
WORDS = {}
FIELDS = {'mig_diff_thresh': {'value': (19, 0)}, 'mig_value_thresh': {}, 'mig_candidate': {'type': (30, 30), 'node': (28, 20), 'page': (17, 0)}}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'MIG_CANDIDATE_VALID_BIT': {'value': 63, 'count': 1, 'stride': 1}, 'MIG_DIFF_THRESH_VALID_BIT': {'value': 63, 'count': 1, 'stride': 1}, 'MIG_VALUE_THRESH_VALID_BIT': {'value': 63, 'count': 1, 'stride': 1}, 'NODE_SHIFT': {'value': 32, 'count': 1, 'stride': 1}, 'REFCNT_COUNT_MASK_PREMIUM': {'value': 524287, 'count': 1, 'stride': 1}, 'REFCNT_COUNT_MASK_STANDARD': {'value': 2047, 'count': 1, 'stride': 1}, 'REFCNT_COUNT_SHIFT': {'value': 5, 'count': 1, 'stride': 1}, 'REFCNT_NODE_STRIDE': {'value': 8, 'count': 1, 'stride': 1}, 'WINDOW_BASE': {'value': 27262976, 'count': 1, 'stride': 1}}
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
