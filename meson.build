project('topdown', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'buildtype=debugoptimized',
    'cpp_std=c++17'
  ])

srcs = [
  'topdown.cpp',
  'application.cpp',
  # states
  'state.cpp',
  'statestack.cpp',
  'titlestate.cpp',
  # graphics
  'font.cpp',
  'glcheck.cpp',
  'rectangleshape.cpp',
  'rendertarget.cpp',
  'shader.cpp',
  'sprite.cpp',
  'stb_image.cpp',
  'texture.cpp',
  'text.cpp',
  'transformable.cpp',
  'view.cpp',
  # system
  'clock.cpp',
  'eventqueue.cpp',
  'utility.cpp',
  'window.cpp',
]

libfreetype2 = dependency('freetype2')
libglm = dependency('glm')
libglew = dependency('glew')
libglfw3 = dependency('glfw3')

deps = [
  libfreetype2,
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
