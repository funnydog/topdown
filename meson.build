project('topdown', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'buildtype=debugoptimized',
    'cpp_std=c++20'
  ])

srcs = [
  'topdown.cpp',
  'application.cpp',
  # states
  'state.cpp',
  'statestack.cpp',
  'titlestate.cpp',
  'menustate.cpp',
  'gamestate.cpp',
  'pausestate.cpp',
  # game
  'world.cpp',
  # 'aircraft.cpp',
  # 'projectile.cpp',
  # 'playercontrol.cpp',
  # graphics
  'font.cpp',
  'glcheck.cpp',
  'rectangleshape.cpp',
  'rendertarget.cpp',
  'shader.cpp',
  'sprite.cpp',
  'stb_image.cpp',
  'texture.cpp',
  'transformable.cpp',
  'view.cpp',
  # system
  'clock.cpp',
  'eventqueue.cpp',
  'utility.cpp',
  'window.cpp',
]

deps = []
deps += dependency('freetype2')
deps += dependency('glm')
deps += dependency('glew')
deps += dependency('glfw3')

exe = executable(
  'topdown',
  sources: srcs,
  dependencies: deps,
  install : true
)

test('basic', exe)
