/*-
 * Copyright (c) 2009 Chris Reinhardt
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $MidnightBSD: src/lib/libmport/version_cmp.c,v 1.3 2008/04/26 17:59:26 ctriv Exp $
 */

#include <stdio.h>
#include <mport.h>
#include <err.h>

#include "private.h"


void query(mportInstance *mport, char **args, int argc) 
{
  mportPackageMeta **packs;
  mportPackageMeta *pkg;
  mportPackageMeta **depends;
    
  if (args[0] == NULL)
    return;
  
  if (mport_pkgmeta_search_master(mport, &packs, "pkg GLOB %Q", args[0]) != MPORT_OK) {
    warnx("Could not execute query for %s: %s", args[0], mport_err_string());
    return;
  }
  
  if (packs == NULL) {
    (void)fprintf(stderr, "No packages installed matching %s\n", args[0]);
    return;
  }
  
  if (*(packs + 1) == NULL) {
    /* only one package matched */
    pkg = *packs;
    
    if (mport_pkgmeta_get_downdepends(mport, pkg, &depends) != MPORT_OK) {
      warnx("Could not populate depends from master database: %s", mport_err_string());
      return;
    }
  
    (void)printf("Package Name:        %s\n", pkg->name);
    (void)printf("Installed version:   %s\n", pkg->version);
    (void)printf("Port origin:         %s\n", pkg->origin);
    (void)printf("Installation prefix: %s\n", pkg->prefix);
    
    if (depends != NULL) {
      (void)printf("Dependencies:\n");
      int i = 0;
      while (depends[i] != NULL) {
        (void)printf("\t%s\n", depends[i]->name);
        i++;
      }      
    }
    
  } else {
    (void)printf("Installed packages matching %s\:\n", args[0]);
    int i;
    for (i=0; packs[i] != NULL; i++) {
      pkg = packs[i];
      (void)printf("\t%s %s\n", pkg->name, pkg->version);
    }
  }
  
  
  (void)printf("\n");
  
  mport_pkgmeta_vec_free(packs);
  
  return;      
}

