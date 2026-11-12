"""generated from defs/xlv.toml by defs-gen - do not edit, change the source
XLV logical volume manager on-disk label structures"""

UNIT = 'xlv'
REGISTERS = {}
WORDS = {'E_W0': {'width': 32, 'grade': 'established'}, 'E_W25': {'width': 32, 'grade': 'established'}, 'E_W27': {'width': 32, 'grade': 'established'}, 'E_W28': {'width': 32, 'grade': 'established'}, 'E_W29': {'width': 32, 'grade': 'established'}, 'E_W42': {'width': 32, 'grade': 'established'}, 'E_W43': {'width': 32, 'grade': 'established'}, 'E_W56': {'width': 32, 'grade': 'established'}, 'E_W57': {'width': 32, 'grade': 'established'}, 'E_W58': {'width': 32, 'grade': 'established'}, 'E_W59': {'width': 32, 'grade': 'established'}, 'E_W60': {'width': 32, 'grade': 'established'}, 'E_W61': {'width': 32, 'grade': 'established'}, 'E_W66': {'width': 32, 'grade': 'established'}, 'E_W67': {'width': 32, 'grade': 'established'}, 'E_W68': {'width': 32, 'grade': 'established'}, 'LBL_W0': {'width': 32, 'grade': 'established'}, 'LBL_W1': {'width': 32, 'grade': 'established'}, 'LBL_W2': {'width': 32, 'grade': 'unknown'}, 'LBL_W3': {'width': 32, 'grade': 'unknown'}, 'LBL_W4': {'width': 32, 'grade': 'established'}, 'LBL_W5': {'width': 32, 'grade': 'established'}, 'MASTER_W0': {'width': 32, 'grade': 'established'}, 'MASTER_W1': {'width': 32, 'grade': 'established'}}
FIELDS = {'E_W0': {'vol_flags': (31, 0)}, 'E_W25': {'sv_flags_hi_no_err_retry': (31, 31), 'sv_flags_hi_other': (30, 16), 'sv_flags_hi_low': (15, 0)}, 'E_W27': {'time': (31, 0)}, 'E_W28': {'vol_attr16': (31, 0)}, 'E_W29': {'sv_type': (31, 24), 'sv_depth': (15, 0)}, 'E_W42': {'sv_nplex': (31, 24), 'plex_index': (23, 16), 'plex_nve': (15, 0)}, 'E_W43': {'plex_flags': (31, 16)}, 'E_W56': {'ve_state': (31, 24), 've_type': (23, 16), 've_nparts': (15, 8), 've_index': (7, 0)}, 'E_W57': {'ve_stripe_unit': (31, 16)}, 'E_W58': {'ve_start_hi': (31, 0)}, 'E_W59': {'ve_start_lo': (31, 0)}, 'E_W60': {'ve_end_hi': (31, 0)}, 'E_W61': {'ve_end_lo': (31, 0)}, 'E_W66': {'part_dev': (31, 0)}, 'E_W67': {'part_size': (31, 0)}, 'E_W68': {'part_index': (31, 24)}, 'LBL_W0': {'magic': (31, 0)}, 'LBL_W1': {'version': (31, 16), 'nparts': (15, 8), 'reserved': (7, 0)}, 'LBL_W2': {'timestamp_hi': (31, 0)}, 'LBL_W3': {'timestamp_lo': (31, 0)}, 'LBL_W4': {'trans_id_hi': (31, 0)}, 'LBL_W5': {'trans_id_lo': (31, 0)}, 'MASTER_W0': {'seq_hi': (31, 0)}, 'MASTER_W1': {'seq_lo_rest': (31, 1), 'seq_lo_slot': (0, 0)}}
ENUMS = {'E_W29.sv_type': {1: 'log', 2: 'data', 3: 'rt'}, 'E_W56.ve_state': {0: 'empty', 1: 'clean', 2: 'active', 3: 'stale', 4: 'offline', 5: 'incomplete'}, 'E_W56.ve_type': {1: 'stripe', 2: 'concat'}}
FIELD_ENUM = {'E_W29.sv_type': 'E_W29.sv_type', 'E_W56.ve_state': 'E_W56.ve_state', 'E_W56.ve_type': 'E_W56.ve_type'}
CONSTANTS = {'ENTRY_OFF': {'value': 288, 'count': 1, 'stride': 1}, 'ENTRY_SIZE': {'value': 280, 'count': 1, 'stride': 1}, 'E_LEN_DATA_SV_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'E_LEN_LOG_SV_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'E_LEN_PART_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'E_LEN_PLEX_NAME': {'value': 32, 'count': 1, 'stride': 1}, 'E_LEN_PLEX_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'E_LEN_RT_SV_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'E_LEN_VE_NAME': {'value': 32, 'count': 1, 'stride': 1}, 'E_LEN_VE_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'E_LEN_VOL_NAME': {'value': 32, 'count': 1, 'stride': 1}, 'E_LEN_VOL_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'E_OFF_DATA_SV_UUID': {'value': 36, 'count': 1, 'stride': 1}, 'E_OFF_LOG_SV_UUID': {'value': 20, 'count': 1, 'stride': 1}, 'E_OFF_PART_UUID': {'value': 248, 'count': 1, 'stride': 1}, 'E_OFF_PLEX_NAME': {'value': 136, 'count': 1, 'stride': 1}, 'E_OFF_PLEX_UUID': {'value': 120, 'count': 1, 'stride': 1}, 'E_OFF_RT_SV_UUID': {'value': 52, 'count': 1, 'stride': 1}, 'E_OFF_VE_NAME': {'value': 192, 'count': 1, 'stride': 1}, 'E_OFF_VE_UUID': {'value': 176, 'count': 1, 'stride': 1}, 'E_OFF_VOL_NAME': {'value': 68, 'count': 1, 'stride': 1}, 'E_OFF_VOL_UUID': {'value': 4, 'count': 1, 'stride': 1}, 'E_SIZE': {'value': 280, 'count': 1, 'stride': 1}, 'HDR_SIZE': {'value': 288, 'count': 1, 'stride': 1}, 'LABEL_BYTES': {'value': 4768, 'count': 1, 'stride': 1}, 'LABEL_MAGIC': {'value': 287316361, 'count': 1, 'stride': 1}, 'LABEL_VERSION': {'value': 2, 'count': 1, 'stride': 1}, 'LBL_LEN_NODENAME': {'value': 264, 'count': 1, 'stride': 1}, 'LBL_OFF_NODENAME': {'value': 24, 'count': 1, 'stride': 1}, 'LBL_SIZE': {'value': 288, 'count': 1, 'stride': 1}, 'MASTER_BYTES': {'value': 512, 'count': 1, 'stride': 1}, 'NAME_LEN': {'value': 32, 'count': 1, 'stride': 1}, 'NODENAME_COPY': {'value': 258, 'count': 1, 'stride': 1}, 'NODENAME_LEN': {'value': 264, 'count': 1, 'stride': 1}, 'NPART': {'value': 16, 'count': 1, 'stride': 1}, 'PTYPE_EFS': {'value': 7, 'count': 1, 'stride': 1}, 'PTYPE_EFS_LEGACY': {'value': 5, 'count': 1, 'stride': 1}, 'PTYPE_RAW': {'value': 3, 'count': 1, 'stride': 1}, 'PTYPE_VOLHDR': {'value': 0, 'count': 1, 'stride': 1}, 'PTYPE_VOLUME': {'value': 6, 'count': 1, 'stride': 1}, 'PTYPE_VXVM': {'value': 14, 'count': 1, 'stride': 1}, 'PTYPE_XFS': {'value': 10, 'count': 1, 'stride': 1}, 'PTYPE_XFSLOG': {'value': 11, 'count': 1, 'stride': 1}, 'PTYPE_XLV': {'value': 12, 'count': 1, 'stride': 1}, 'PTYPE_XVM': {'value': 13, 'count': 1, 'stride': 1}}
TESTS = [('E_W25', 2147483648, {'sv_flags_hi_no_err_retry': 1, 'sv_flags_hi_other': 0, 'sv_flags_hi_low': 0})]


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
