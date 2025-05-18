project('topdown', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'buildtype=debugoptimized',
    'cpp_std=c++20'
  ])

srcs = [
  'main.cpp',
  'application.cpp',
  # states
  'statestack.cpp',
  'titlestate.cpp',
  'menustate.cpp',
  'gamestate.cpp',
  'pausestate.cpp',
  # graphics
  'font.cpp',
  'glcheck.cpp',
  'rect.cpp',
  'rectangleshape.cpp',
  'rendertarget.cpp',
  'shader.cpp',
  'stb_image.cpp',
  'texture.cpp',
  'transformable.cpp',
  # system
  'clock.cpp',
  'eventqueue.cpp',
  'utility.cpp',
  'window.cpp',
]

deps = []
deps += dependency('freetype2', required : true, fallback : ['freetype2', 'freetype_dep'])
deps += dependency('glm', required : true, fallback : ['glm', 'glm_dep'])
deps += dependency('glew', required : true, fallback : ['glew', 'glew_dep'])
deps += dependency('glfw3', required : true, fallback : ['glfw', 'glfw_dep'])

exe = executable(
  'topdown',
  sources: srcs,
  dependencies: deps,
  install : true
)

test('basic', exe)
