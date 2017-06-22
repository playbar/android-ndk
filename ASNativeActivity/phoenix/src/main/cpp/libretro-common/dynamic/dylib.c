/* Copyright  (C) 2010-2017 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this file (dylib.c).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include <stdio.h>
#include <dynamic/dylib.h>

#ifdef NEED_DYNAMIC

#include <dlfcn.h>
#include <src/callstack.h>

int is_file_exist(const char *file_path)
{
   if(file_path == NULL )
   {
      return  -1;
   }
   if( access(file_path, F_OK) == 0 )
      return 0;
   return -1;
}



/**
 * dylib_load:
 * @path                         : Path to libretro core library.
 *
 * Platform independent dylib loading.
 *
 * Returns: library handle on success, otherwise NULL.
 **/
dylib_t dylib_load(const char *path)
{
//   FILE *pFile = fopen(path, "rb");
//   FILE *pOut = fopen("/sdcard/2048_libretro_android.so","wb");
//   char tmp[1024] = {0};
//   while( true){
//      int len = fread(tmp, 1, 1024, pFile);
//      if( len <= 0 ){
//         break;
//      }
//      fwrite(tmp, 1, len, pOut );
//   }
//   fclose(pFile);
//   fclose(pOut);

   bool b = is_file_exist(path);
   ////
   dylib_t lib = dlopen(path, RTLD_LAZY);
   return lib;
}

char *dylib_error(void)
{
#ifdef _WIN32
   if (last_dyn_error[0])
      return last_dyn_error;
   return NULL;
#else
   return (char*)dlerror();
#endif
}

function_t dylib_proc(dylib_t lib, const char *proc)
{
   function_t sym;

#ifdef _WIN32
   sym = (function_t)GetProcAddress(lib ?
         (HMODULE)lib : GetModuleHandle(NULL), proc);
   if (!sym)
   {
      set_dl_error();
      return NULL;
   }
   last_dyn_error[0] = 0;
#else
   void *ptr_sym = NULL;

   if (lib)
      ptr_sym = dlsym(lib, proc);
   else
   {
      void *handle = dlopen(NULL, RTLD_LAZY);
      if (handle)
      {
         ptr_sym = dlsym(handle, proc);
         dlclose(handle);
      }
   }

   /* Dirty hack to workaround the non-legality of
    * (void*) -> fn-pointer casts. */
   memcpy(&sym, &ptr_sym, sizeof(void*));
#endif

   return sym;
}

/**
 * dylib_close:
 * @lib                          : Library handle.
 *
 * Frees library handle.
 **/
void dylib_close(dylib_t lib)
{
#ifdef _WIN32
   if (!FreeLibrary((HMODULE)lib))
      set_dl_error();
   last_dyn_error[0] = 0;
#else
#ifndef NO_DLCLOSE
   dlclose(lib);
#endif
#endif
}

#endif
