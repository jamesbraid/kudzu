"""generated from formats/gbe_board/format.toml by defs-gen - do not edit, change the source
Board description bytes the O2 kernel reads to choose GBE timing"""

UNIT = 'gbe_board'
REGISTERS = {}
WORDS = {}
FIELDS = {}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'BOARD_INTERLACE_OFF': {'value': 45, 'count': 1, 'stride': 1}, 'BOARD_PANEL_OFF': {'value': 46, 'count': 1, 'stride': 1}, 'PANEL_SETUP_A': {'value': 130, 'count': 1, 'stride': 1}, 'PANEL_SETUP_B': {'value': 131, 'count': 1, 'stride': 1}, 'PANEL_SETUP_C': {'value': 132, 'count': 1, 'stride': 1}}
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
