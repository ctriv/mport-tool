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

#include "private.h"


void delete(mportInstance *mport, char **args, int argc) 
{
  mportPackageMeta **pkgs;
  int i;
  
  if (args[0] == NULL)
    return;
  
  if (mport_pkgmeta_search_master(mport, &pkgs, "pkg GLOB %Q", args[0]) != MPORT_OK) {
    (void)fprintf(stderr, "Could not execute query for %s: %s\n", args[0], mport_err_string());
    return;
  }
  
  if (pkgs == NULL) {
    (void)fprintf(stderr, "No packages installed matching %s\n", args[0]);
    return;
  }
  
  for (i=0; pkgs[i] != NULL; i++) {
    if (mport_delete_primative(mport, pkgs[i], 0)) 
      (void)fprintf(stderr, "Could not delete %s-%s: %s\n", pkgs[i]->name, pkgs[i]->version, mport_err_string());
  }

  mport_pkgmeta_vec_free(pkgs);
  
  return;      
}

