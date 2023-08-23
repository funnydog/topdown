project('topdown', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'buildtype=debugoptimized',
    'cpp_std=c++17'
  ])

srcs = [
  'glcheck.cpp',
  'clock.cpp',
  'rendertarget.cpp',
  'shader.cpp',
  'stb_image.cpp',
  'texture.cpp',
  'topdown.cpp',
  'utility.cpp',
]

libglm = dependency('glm')
libglew = dependency('glew')
libglfw3 = dependency('glfw3')

deps = [
  libglm,
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
