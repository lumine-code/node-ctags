{
  'targets': [
    {
      'target_name': 'ctags',
      'include_dirs': [ '<!(node -p "require(\'node-addon-api\').include_dir")' ],
      'defines': [ 'NAPI_VERSION=8' ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      },
      'sources': [
        'src/readtags.c',
        'src/tags.cc',
        'src/tag-finder.cc',
        'src/tag-reader.cc'
      ],
      'conditions': [
        ['OS=="win"', {
          'msvs_disabled_warnings': [
            4267,  # conversion from 'size_t' to 'int', possible loss of data
            4530,  # C++ exception handler used, but unwind semantics are not enabled
            4506,  # no definition for inline function
          ],
        }],
      ],
    }
  ]
}
