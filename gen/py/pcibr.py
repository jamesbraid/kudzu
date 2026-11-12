"""generated from defs/pcibr.toml by defs-gen - do not edit, change the source
PCI bridge widget identification and window layouts"""

UNIT = 'pcibr'
REGISTERS = {}
WORDS = {'WID': {'width': 32, 'grade': 'established'}}
FIELDS = {'WID': {'part': (27, 12), 'field11': (11, 1)}}
ENUMS = {'WID.part': {49154: 'layout32', 53250: 'layout32b', 53506: 'layout64b', 53522: 'layout64bx', 57344: 'layout64a', 57360: 'layout64ax'}, 'WID.field11': {36: 'value36', 54: 'value54'}}
FIELD_ENUM = {'WID.part': 'WID.part', 'WID.field11': 'WID.field11'}
CONSTANTS = {'CFG_FUNC_SHIFT': {'value': 8, 'count': 1, 'stride': 1}, 'CFG_SLOTS_32': {'value': 8, 'count': 1, 'stride': 1}, 'CFG_SLOT_BIAS_64': {'value': 1, 'count': 1, 'stride': 1}, 'CFG_SLOT_SHIFT': {'value': 12, 'count': 1, 'stride': 1}, 'CFG_WINDOW32_OFFSET': {'value': 131072, 'count': 1, 'stride': 1}, 'INTR_ADDR32_HI': {'value': 17, 'count': 1, 'stride': 1}, 'INTR_ADDR32_LO': {'value': 8, 'count': 1, 'stride': 1}, 'INTR_ADDR32_OFFSET': {'value': 308, 'count': 1, 'stride': 1}, 'INTR_ADDR32_SRC_HI': {'value': 47, 'count': 1, 'stride': 1}, 'INTR_ADDR32_SRC_LO': {'value': 38, 'count': 1, 'stride': 1}, 'INTR_ADDR64_OFFSET': {'value': 304, 'count': 1, 'stride': 1}, 'INTR_COUNT': {'value': 8, 'count': 1, 'stride': 1}, 'INTR_STRIDE': {'value': 8, 'count': 1, 'stride': 1}, 'WID_OFFSET': {'value': 4, 'count': 1, 'stride': 1}, 'WINDOW_BIT_A': {'value': 63, 'count': 1, 'stride': 1}, 'WINDOW_BIT_B': {'value': 60, 'count': 1, 'stride': 1}, 'WINDOW_BIT_C': {'value': 57, 'count': 1, 'stride': 1}}
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
