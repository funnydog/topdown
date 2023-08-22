project('topdown', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'buildtype=debugoptimized',
    'cpp_std=c++17'
  ])

srcs = [
  'glcheck.cpp',
  'topdown.cpp',
  'utility.cpp',
]

libglew = dependency('glew')
libglfw3 = dependency('glfw3')

deps = [
  libglew,
  libglfw3,
]

exe = executable(
  'topdown',
  sources: srcs,
  dependencies: deps,
  install : true
)

test('basic', exe)
