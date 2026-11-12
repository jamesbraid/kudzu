"""generated from defs/tg.toml by defs-gen - do not edit, change the source
gigabit Ethernet controller match table of the Origin 300/350 driver"""

UNIT = 'tg'
REGISTERS = {}
WORDS = {'ROW_W0': {'width': 32, 'grade': 'established'}, 'ROW_W1': {'width': 32, 'grade': 'established'}}
FIELDS = {'ROW_W0': {'vendor': (31, 16), 'device': (15, 0)}, 'ROW_W1': {'subsys_vendor': (31, 16), 'subsys_device': (15, 0)}}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'CFG_OFF_DEVICE': {'value': 0, 'count': 1, 'stride': 1}, 'CFG_OFF_SUBSYS_DEVICE': {'value': 46, 'count': 1, 'stride': 1}, 'CFG_OFF_SUBSYS_VENDOR': {'value': 44, 'count': 1, 'stride': 1}, 'CFG_OFF_VENDOR': {'value': 2, 'count': 1, 'stride': 1}, 'MATCH_ROW_COUNT': {'value': 5, 'count': 1, 'stride': 1}, 'MATCH_ROW_STRIDE': {'value': 16, 'count': 1, 'stride': 1}, 'PCI_DEVICE_1645': {'value': 5701, 'count': 1, 'stride': 1}, 'PCI_DEVICE_1648': {'value': 5704, 'count': 1, 'stride': 1}, 'PCI_DEVICE_16A8': {'value': 5800, 'count': 1, 'stride': 1}, 'PCI_SUBSYS_DEVICE_FIRST': {'value': 32784, 'count': 1, 'stride': 1}, 'PCI_SUBSYS_VENDOR': {'value': 4265, 'count': 1, 'stride': 1}, 'PCI_VENDOR': {'value': 5348, 'count': 1, 'stride': 1}}
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
