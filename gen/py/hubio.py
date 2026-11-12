"""generated from defs/hubio.toml by defs-gen - do not edit, change the source
Origin 300 hub crosstalk-side register window"""

UNIT = 'hubio'
REGISTERS = {'llp_csr': {'offset': 296, 'access': 'r', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'flags': {'offset': 488, 'access': 'r', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'crb_a': {'offset': 1024, 'access': 'r', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'crb_b': {'offset': 1032, 'access': 'r', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'crb_c': {'offset': 1040, 'access': 'r', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}}
WORDS = {}
FIELDS = {'llp_csr': {'bit13': (13, 13)}, 'flags': {'bit4': (4, 4), 'bit0': (0, 0)}, 'crb_a': {'bit55': (55, 55), 'type': (54, 52), 'bit51': (51, 51), 'bit50': (50, 50), 'bit49': (49, 49), 'bit0': (0, 0)}, 'crb_b': {'engine_alt': (58, 58), 'sub': (42, 41), 'kind': (32, 30)}, 'crb_c': {'bit48': (48, 48)}}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'CRB_COUNT': {'value': 15, 'count': 1, 'stride': 1}, 'CRB_STRIDE': {'value': 32, 'count': 1, 'stride': 1}, 'NODE_SHIFT': {'value': 33, 'count': 1, 'stride': 1}, 'WIDGET_PART_C104': {'value': 49412, 'count': 1, 'stride': 1}, 'WIDGET_PART_C105': {'value': 49413, 'count': 1, 'stride': 1}, 'WINDOW_BASE': {'value': 29360128, 'count': 1, 'stride': 1}}
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
