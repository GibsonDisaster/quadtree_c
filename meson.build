project('quadtree', 'c', version: '0.1', default_options: ['warning_level=3'])

raylib_dep = dependency('raylib')

src = ['main.c', 'quadtree.c']

exe = executable('quadtree', src, dependencies: [raylib_dep], install: true)

test('basic', exe)
