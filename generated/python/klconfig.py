"""generated from formats/klconfig/format.toml by defs-gen - do not edit, change the source
hardware configuration board record"""

UNIT = 'klconfig'
REGISTERS = {}
WORDS = {'BRD_W0': {'width': 32, 'grade': 'established'}, 'BRD_W1': {'width': 32, 'grade': 'inferred'}, 'BRD_W2': {'width': 32, 'grade': 'established'}, 'BRD_W3': {'width': 32, 'grade': 'established'}, 'BRD_W5': {'width': 32, 'grade': 'established'}, 'BRD_W8': {'width': 32, 'grade': 'established'}, 'COMP_W0': {'width': 32, 'grade': 'established'}, 'COMP_W6': {'width': 32, 'grade': 'established'}, 'CONS_W2': {'width': 32, 'grade': 'established'}, 'CONS_W3': {'width': 32, 'grade': 'established'}, 'CONS_W4': {'width': 32, 'grade': 'established'}, 'CONS_W5': {'width': 32, 'grade': 'established'}, 'CONS_W8': {'width': 32, 'grade': 'established'}, 'GDA_W97': {'width': 32, 'grade': 'established'}, 'MEM_W14': {'width': 32, 'grade': 'established'}, 'NODECLASS_W1': {'width': 32, 'grade': 'established'}, 'PEPAIR_W0': {'width': 32, 'grade': 'established'}}
FIELDS = {'BRD_W0': {'next': (31, 0)}, 'BRD_W1': {'class': (23, 20), 'type_low': (19, 16)}, 'BRD_W2': {'flags_master_baseio': (21, 21), 'slot': (15, 8)}, 'BRD_W3': {'module': (15, 0)}, 'BRD_W5': {'numcompts': (7, 0)}, 'BRD_W8': {'nasid': (31, 16)}, 'COMP_W0': {'struct_type': (31, 24), 'flags_enabled': (8, 8)}, 'COMP_W6': {'physid': (31, 24)}, 'CONS_W2': {'config_base_hi': (31, 0)}, 'CONS_W3': {'config_base_lo': (31, 0)}, 'CONS_W4': {'memory_base_hi': (31, 0)}, 'CONS_W5': {'memory_base_widget': (31, 24), 'memory_base_rest': (23, 0)}, 'CONS_W8': {'nasid': (31, 16), 'npci': (7, 0)}, 'GDA_W97': {'pebrick_marker': (31, 0)}, 'MEM_W14': {'dimm_select': (15, 0)}, 'NODECLASS_W1': {'class': (15, 12)}, 'PEPAIR_W0': {'pe_nasid': (31, 16), 'host_nasid': (15, 0)}}
ENUMS = {'BRD_W1.class': {1: 'node', 2: 'io', 3: 'router', 5: 'graphics'}, 'COMP_W0.struct_type': {2: 'hub', 3: 'memory', 5: 'io', 9: 'router', 10: 'graphics'}, 'NODECLASS_W1.class': {12: 'pebrick'}}
FIELD_ENUM = {'BRD_W1.class': 'BRD_W1.class', 'COMP_W0.struct_type': 'COMP_W0.struct_type', 'NODECLASS_W1.class': 'NODECLASS_W1.class'}
CONSTANTS = {'BRD_OFF_COMPTS': {'value': 36, 'count': 1, 'stride': 1}, 'BRD_STRIDE_COMPTS': {'value': 4, 'count': 1, 'stride': 1}, 'DIR_CONFIG_OFFSET_MASK': {'value': 1099511627775, 'count': 1, 'stride': 1}, 'DIR_OFF': {'value': 8192, 'count': 1, 'stride': 1}, 'DIR_OFF_CONFIG_OFFSET': {'value': 72, 'count': 1, 'stride': 1}, 'GDA_COUNT_NASIDTABLE_IP27': {'value': 64, 'count': 1, 'stride': 1}, 'GDA_COUNT_NASIDTABLE_IP35': {'value': 128, 'count': 1, 'stride': 1}, 'GDA_COUNT_PEBRICK': {'value': 4, 'count': 1, 'stride': 1}, 'GDA_OFF_NASIDTABLE': {'value': 128, 'count': 1, 'stride': 1}, 'GDA_OFF_PEBRICK': {'value': 392, 'count': 1, 'stride': 1}, 'GDA_PEBRICK_MAGIC': {'value': 1346724459, 'count': 1, 'stride': 1}, 'GDA_STRIDE_NASIDTABLE': {'value': 2, 'count': 1, 'stride': 1}, 'GDA_STRIDE_PEBRICK': {'value': 4, 'count': 1, 'stride': 1}, 'HDR_OFF_BOARD_INFO': {'value': 20, 'count': 1, 'stride': 1}, 'MEM_COUNT_BNKSZ': {'value': 8, 'count': 1, 'stride': 1}, 'MEM_OFF_BNKSZ': {'value': 60, 'count': 1, 'stride': 1}, 'MEM_STRIDE_BNKSZ': {'value': 2, 'count': 1, 'stride': 1}, 'NODECLASS_OFF': {'value': 67108864, 'count': 1, 'stride': 1}, 'NODECLASS_SPACE_LOCAL': {'value': 268435456, 'count': 1, 'stride': 1}, 'NODECLASS_SPACE_REMOTE': {'value': 805306368, 'count': 1, 'stride': 1}}
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
