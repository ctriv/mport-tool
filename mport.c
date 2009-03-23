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

#include <sys/cdefs.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
// #include <mport.h>
#include <histedit.h>

#include "private.h"

volatile sig_atomic_t gotsig = 0;

static  unsigned char   complete(EditLine *, int);
static  char   *prompt(EditLine *);
static  void    sig(int);

static char * prompt(EditLine *el)
{  
  static char a[] = "mport> ";
  
  return a; 
}
                        
static void sig(int i)
{
  gotsig = i;
}
                                

int main (int argc, char **argv)
{
  EditLine *el = NULL;
  int num;
  const char *buf;
  Tokenizer *tok;
  History *hist;
  HistEvent ev;

  (void) signal(SIGINT, sig); 
  (void) signal(SIGQUIT, sig);
  (void) signal(SIGHUP, sig); 
  (void) signal(SIGTERM, sig);

  hist = history_init();        
  history(hist, &ev, H_SETSIZE, 100);

  tok  = tok_init(NULL);

  el = el_init(*argv, stdin, stdout, stderr);
  el_set(el, EL_EDITOR, "emacs");    
  el_set(el, EL_SIGNAL, 1);       
  el_set(el, EL_PROMPT, prompt);  

  el_set(el, EL_HIST, history, hist);

  el_set(el, EL_ADDFN, "ed-complete", "Complete argument", complete);
  el_set(el, EL_BIND, "^I", "ed-complete", NULL);

  el_source(el, "mport");
  
  while ((buf = el_gets(el, &num)) != NULL && num != 0)  {
    int ac, cc, co;
    const char **av;
    const LineInfo *li;
  
    li = el_line(el);

    if (gotsig) {
      (void) fprintf(stderr, "Got signal %d.\n", gotsig);
      gotsig = 0;
      el_reset(el);
    }

    ac = cc = co = 0;
    if (tok_line(tok, li, &ac, &av, &cc, &co) < 0) {
      (void) fprintf(stderr, "Internal error\n");
      continue;
    }

    history(hist, &ev, H_ENTER, buf);

    (void)printf("av[0]: %s\n", av[0]);
    
    if (strcmp(av[0], "query") == 0) {
      (void)printf("query!!\n");
    } else if (strcmp(av[0], "help") == 0) {
      (void)printf("query or help please!\n");
    } else {
      (void)printf("nope!\n");
    }
    
    tok_reset(tok);
  }

  el_end(el);
  tok_end(tok);
  history_end(hist);

  return (0);
}

static unsigned char complete(EditLine *el, int key) 
{
  el_insertstr(el, "you hit tab");
  return CC_REFRESH;
}

