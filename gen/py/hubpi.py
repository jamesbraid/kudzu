"""generated from defs/hubpi.toml by defs-gen - do not edit, change the source
node hub interrupt pending and mask registers"""

UNIT = 'hubpi'
REGISTERS = {'int_pend_mod': {'offset': 144, 'access': 'w', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'int_pend0': {'offset': 152, 'access': 'r', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'int_pend1': {'offset': 160, 'access': 'r', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'int_mask0_a': {'offset': 168, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'int_mask1_a': {'offset': 176, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'int_mask0_b': {'offset': 184, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'int_mask1_b': {'offset': 192, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}}
WORDS = {}
FIELDS = {'int_pend_mod': {'set': (8, 8), 'level': (6, 0)}, 'int_pend0': {}, 'int_pend1': {}, 'int_mask0_a': {}, 'int_mask1_a': {}, 'int_mask0_b': {}, 'int_mask1_b': {}}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'INT_PEND_MOD_ALT_OFFSET': {'value': 2097296, 'count': 1, 'stride': 1}, 'LOCAL_WINDOW_BASE': {'value': 16777216, 'count': 1, 'stride': 1}, 'NODE_SHIFT': {'value': 32, 'count': 1, 'stride': 1}, 'NODE_WINDOW_BASE': {'value': 25165824, 'count': 1, 'stride': 1}}
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
