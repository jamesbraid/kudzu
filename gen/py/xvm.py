"""generated from defs/xvm.toml by defs-gen - do not edit, change the source
XVM volume manager on-disk label structures"""

UNIT = 'xvm'
REGISTERS = {}
WORDS = {'ATTR_PERM_W6': {'width': 32, 'grade': 'established'}, 'ATTR_PERM_W7': {'width': 32, 'grade': 'established'}, 'ATTR_PERM_W8': {'width': 32, 'grade': 'established'}, 'ATTR_W0': {'width': 32, 'grade': 'inferred'}, 'ATTR_W1': {'width': 32, 'grade': 'established'}, 'LH_W0': {'width': 32, 'grade': 'established'}, 'LH_W1': {'width': 32, 'grade': 'established'}, 'PL_W0': {'width': 32, 'grade': 'established'}, 'PL_W1': {'width': 32, 'grade': 'inferred'}, 'PL_W14': {'width': 32, 'grade': 'established'}, 'PL_W15': {'width': 32, 'grade': 'established'}, 'PL_W16': {'width': 32, 'grade': 'established'}, 'PL_W17': {'width': 32, 'grade': 'established'}, 'PL_W30': {'width': 32, 'grade': 'established'}, 'VE_MIRROR_W14': {'width': 32, 'grade': 'established'}, 'VE_MIRROR_W15': {'width': 32, 'grade': 'established'}, 'VE_MIRROR_W16': {'width': 32, 'grade': 'inferred'}, 'VE_RAID_W14': {'width': 32, 'grade': 'inferred'}, 'VE_RAID_W15': {'width': 32, 'grade': 'inferred'}, 'VE_SLICE_W14': {'width': 32, 'grade': 'established'}, 'VE_SLICE_W15': {'width': 32, 'grade': 'established'}, 'VE_SLICE_W16': {'width': 32, 'grade': 'established'}, 'VE_STRIPE_W14': {'width': 32, 'grade': 'established'}, 'VE_SUBVOL_W14': {'width': 32, 'grade': 'established'}, 'VE_VOL_W14': {'width': 32, 'grade': 'established'}, 'VE_W0': {'width': 32, 'grade': 'established'}, 'VE_W1': {'width': 32, 'grade': 'established'}, 'VE_W10': {'width': 32, 'grade': 'established'}, 'VE_W11': {'width': 32, 'grade': 'established'}, 'VE_W12': {'width': 32, 'grade': 'established'}, 'VE_W13': {'width': 32, 'grade': 'established'}}
FIELDS = {'ATTR_PERM_W6': {'uid': (31, 0)}, 'ATTR_PERM_W7': {'gid': (31, 0)}, 'ATTR_PERM_W8': {'mode': (31, 0)}, 'ATTR_W0': {'type': (31, 24), 'flag_persistent': (16, 16)}, 'ATTR_W1': {'tid': (31, 0)}, 'LH_W0': {'magic': (31, 0)}, 'LH_W1': {'version': (31, 16), 'type': (15, 8), 'size': (7, 0)}, 'PL_W0': {'magic': (31, 0)}, 'PL_W1': {'version': (31, 16), 'state_draining': (14, 14), 'state_cluster': (13, 13), 'state_private': (12, 12), 'state_recovering': (11, 11), 'state_mediaerror': (10, 10), 'state_spare': (9, 9), 'state_online': (8, 8), 'flags_sync': (3, 3), 'flags_duplicate': (2, 2), 'flags_bootable': (1, 1)}, 'PL_W14': {'secsize': (31, 16), 'xvmlabel': (15, 0)}, 'PL_W15': {'xvmlabelblks': (31, 16), 'dbn': (15, 0)}, 'PL_W16': {'nblks': (31, 0)}, 'PL_W17': {'timestamp': (31, 0)}, 'PL_W30': {'clusterid': (31, 0)}, 'VE_MIRROR_W14': {'oldlength_hi': (31, 0)}, 'VE_MIRROR_W15': {'oldlength_lo': (31, 0)}, 'VE_MIRROR_W16': {'logfreq': (31, 16), 'policy': (15, 8), 'config': (7, 0)}, 'VE_RAID_W14': {'susize': (31, 0)}, 'VE_RAID_W15': {'layout': (31, 24)}, 'VE_SLICE_W14': {'startblk_hi': (31, 0)}, 'VE_SLICE_W15': {'startblk_lo': (31, 0)}, 'VE_SLICE_W16': {'seq': (31, 16), 'flags': (15, 8), 'partition': (7, 0)}, 'VE_STRIPE_W14': {'susize': (31, 0)}, 'VE_SUBVOL_W14': {'svtype': (31, 24)}, 'VE_VOL_W14': {'repfull': (31, 24)}, 'VE_W0': {'type': (31, 24), 'state_disabled': (20, 20), 'state_valid': (17, 17), 'state_clean': (16, 16), 'pieces': (15, 0)}, 'VE_W1': {'tid': (31, 0)}, 'VE_W10': {'length_hi': (31, 0)}, 'VE_W11': {'length_lo': (31, 0)}, 'VE_W12': {'pos': (31, 16), 'cowfirst': (15, 0)}, 'VE_W13': {'cownext': (31, 16), 'cowregsz': (15, 0)}}
ENUMS = {'ATTR_W0.type': {0: 'free', 1: 'name', 2: 'perm'}, 'LH_W1.type': {0: 'free', 1: 'reserved', 2: 've', 3: 'attr'}, 'VE_MIRROR_W16.policy': {0: 'round_robin', 1: 'sequential'}, 'VE_SUBVOL_W14.svtype': {0: 'all', 1: 'log', 2: 'data', 3: 'rt', 4: 'swap', 16: 'user1'}, 'VE_W0.type': {0: 'free', 1: 'slice', 2: 'concat', 3: 'stripe', 4: 'raid', 5: 'mirror', 6: 'subvol', 7: 'volume', 11: 'cow', 12: 'snapshot'}}
FIELD_ENUM = {'ATTR_W0.type': 'ATTR_W0.type', 'LH_W1.type': 'LH_W1.type', 'VE_MIRROR_W16.policy': 'VE_MIRROR_W16.policy', 'VE_SUBVOL_W14.svtype': 'VE_SUBVOL_W14.svtype', 'VE_W0.type': 'VE_W0.type'}
CONSTANTS = {'ATTR_LEN_NAME': {'value': 48, 'count': 1, 'stride': 1}, 'ATTR_LEN_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'ATTR_OFF_NAME': {'value': 24, 'count': 1, 'stride': 1}, 'ATTR_OFF_UUID': {'value': 8, 'count': 1, 'stride': 1}, 'ATTR_SIZE': {'value': 72, 'count': 1, 'stride': 1}, 'DISKNAME_MAX': {'value': 48, 'count': 1, 'stride': 1}, 'HOSTNAME_MAX': {'value': 32, 'count': 1, 'stride': 1}, 'LABELHDR_MAGIC': {'value': 1818519666, 'count': 1, 'stride': 1}, 'LABELHDR_VERSION': {'value': 2, 'count': 1, 'stride': 1}, 'LABELHDR_VERSION_LEGACY': {'value': 1, 'count': 1, 'stride': 1}, 'LABENT_SIZE': {'value': 72, 'count': 1, 'stride': 1}, 'LH_SIZE': {'value': 8, 'count': 1, 'stride': 1}, 'NAME_MAX': {'value': 48, 'count': 1, 'stride': 1}, 'PHYSLAB_GPT_FIRST_PARTITION_INDEX': {'value': 0, 'count': 1, 'stride': 1}, 'PHYSLAB_GPT_MIN_PARTITIONS': {'value': 2, 'count': 1, 'stride': 1}, 'PHYSLAB_MAGIC': {'value': 2020368738, 'count': 1, 'stride': 1}, 'PHYSLAB_START_BLOCK': {'value': 1, 'count': 1, 'stride': 1}, 'PHYSLAB_VERSION': {'value': 2, 'count': 1, 'stride': 1}, 'PHYSLAB_VERSION_LEGACY': {'value': 1, 'count': 1, 'stride': 1}, 'PL_LEN_DISKNAME': {'value': 48, 'count': 1, 'stride': 1}, 'PL_LEN_HOSTNAME': {'value': 32, 'count': 1, 'stride': 1}, 'PL_LEN_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'PL_OFF_DISKNAME': {'value': 72, 'count': 1, 'stride': 1}, 'PL_OFF_HOSTNAME': {'value': 8, 'count': 1, 'stride': 1}, 'PL_OFF_UUID': {'value': 40, 'count': 1, 'stride': 1}, 'PL_SIZE': {'value': 124, 'count': 1, 'stride': 1}, 'VE_COW_LEN_RUUID': {'value': 16, 'count': 1, 'stride': 1}, 'VE_COW_OFF_RUUID': {'value': 56, 'count': 1, 'stride': 1}, 'VE_LEN_CHILDUUID': {'value': 16, 'count': 1, 'stride': 1}, 'VE_LEN_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'VE_OFF_CHILDUUID': {'value': 24, 'count': 1, 'stride': 1}, 'VE_OFF_UUID': {'value': 8, 'count': 1, 'stride': 1}, 'VE_SIZE': {'value': 72, 'count': 1, 'stride': 1}}
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
