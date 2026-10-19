#BREAK#
s({
    'name': 'scroll template',
    'desc': 'template has been made for learning purposes.',
    'id': 0,
    'type': None,
    'func_activate': general_scrolls.activate_scroll_template,
    'func_build':    general_scrolls.build_scroll_template,
    'func_kwargs': {},
    'func_tuples': [],
    'default_technique_lock': True,
    'default_technique_id': 0, # 0 is the technique template id
    'technique': None, # set on build
    'default_modifier_locks': {
        0: True
    },
    'default_modifier_ids': {
        0: 0, # 0 is the modifier template id
    },
    'max_modifiers': 1,
    'modifiers_dict': {},
})
#BREAK#
s({
    'name': 'Normal Scroll',
    'desc': 'Player Explodes dealing damage in radius.',
    'id': 1,
    'type': None,
    'func_activate': general_scrolls.activate_scroll_Normal,
    'func_build':    general_scrolls.build_scroll_Normal,
    'func_kwargs': {},
    'func_tuples': [],
    'default_technique_lock': True,
    'default_technique_id': None,
    'default_modifier_locks': {
        0: True
    },
    'default_modifier_ids': {
        0: None,
    },
    'technique': None, # set on build
    'max_modifiers': 0,
    'modifiers_dict': {},
})
#BREAK#
