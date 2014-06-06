/*
 * Copyright 1999, 2005 Brown University, Providence, RI.
 * 
 *                         All Rights Reserved
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose other than its incorporation into a
 * commercial product is hereby granted without fee, provided that the
 * above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Brown University not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 * 
 * BROWN UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR ANY
 * PARTICULAR PURPOSE.  IN NO EVENT SHALL BROWN UNIVERSITY BE LIABLE FOR
 * ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

//#include <sys/resource.h>
#include "extraMain.h"
#include <vector>
#include <list>
#include "Item.h"
#include "MeChart.h"
#include "headFinder.h"
#include "ClassRule.h"

void
generalInit(ECString path)
{
   /*
  struct rlimit 	core_limits;
  core_limits.rlim_cur = 0;
  core_limits.rlim_max = 0;
  setrlimit( RLIMIT_CORE, &core_limits );

  struct rlimit stack_limits;
  stack_limits.rlim_cur = 0;
  stack_limits.rlim_max = 0;
  getrlimit( RLIMIT_STACK, &stack_limits );
  if (stack_limits.rlim_cur < stack_limits.rlim_max)
    {
      stack_limits.rlim_cur = stack_limits.rlim_max;
      setrlimit( RLIMIT_STACK, &stack_limits );
    }
    */
  Term::init( path );
  readHeadInfo(path);
  InputTree::init();
  UnitRules* ur = new UnitRules;
  ur->readData(path); 
  Bchart::unitRules = ur;
  Bchart::readTermProbs(path);
  MeChart::init(path);
  Bchart::setPosStarts();
  ChartBase::midFactor = (float)( (1.0 - (.3684 *ChartBase::endFactor))/(1.0 - .3684) );
  if(Feature::isLM) ClassRule::readCRules(path);
}

InputTree*
inputTreeFromBsts(Val* at, short& pos, SentRep& sr)
{
  //cerr << "itfat " << at->trm() << " " << at->bsts().size() << endl;
  short trmInt = at->trm();
  if(trmInt >= 400)
    {
      cerr << "Bad trm int: " << trmInt << endl;
      assert(trmInt < 400);
    }
  const Term* trm = NULL;
  ECString trmString;
  if(trmInt >= 0)
    {
      trm = Term::fromInt(trmInt);
      trmString = trm->name();
    }
  ECString wrdString;
  ECString ntString;
  list<InputTree*> subtrs;
  InputTree* ans;
  short begn = pos;
  if(trm && trm->terminal_p() && at->status == TERMINALVAL)
    {
      wrdString = sr[pos].lexeme();
      pos++;
    }
  else
    {
      assert(at);
      Bsts::iterator bi = at->bsts().begin();
      int vpos = 0;
      for( ; bi != at->bsts().end() ; bi++)
        {
          Bst& sb = **bi;
          int vval = at->vec()[vpos];
          if(vval >= sb.num())
            {
              cerr << vpos << " "
                   << vval << " " << sb.num() << " " << *at << endl;
              assert(vval < sb.num());
            }
          InputTree* sit = inputTreeFromBsts(sb.nth(vval), pos,sr);
          assert(sit);
          subtrs.push_back(sit);
          vpos++;
        }
    }

  /* bestParse in MeChart creates a ficticious level of structure that
     this removes*/

  //if(!trm && !at->edge() && at->status == EXTRAVAL) return subtrs.front();
  if(!at->edge() && at->status == EXTRAVAL) return subtrs.front();

  ans = new InputTree(begn, pos, wrdString, trmString, ntString,
                      subtrs, NULL, NULL);
  
  
  /* This code inserts the position of the head word after the
     non-terminal */
  /*
  if(!trm->terminal_p())
    {
      int hp = headPosFromTree(ans);
      assert(hp >= 0);
      ans->ntInfo() = intToString(hp);
    }
  */
  InputTreesIter iti = subtrs.begin();
  for( ; iti != subtrs.end() ; iti++) (*iti)->parentSet() = ans;


  //cerr << "ITF " << *ans << endl;
  return ans;
}
