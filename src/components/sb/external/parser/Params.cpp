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

#include "Params.h"
#include "Bchart.h"
#include "CntxArray.h"
#include "ClassRule.h"
#include <cstring>

void
Params::
init( ECArgs& args ) 
{  
   fileString_ = args.arg(0);

   if( args.nargs() > 2 || args.nargs() == 0 )	// require path name 
     error( "Needs 1 or 2 args." );

   if(args.isset('M'))
     {
       Feature::setLM();
       CntxArray::sz = 6;
     }
   if(args.isset('N'))
     {
       Bchart::Nth = atoi(args.value('N').c_str());
       //cerr << "Set Nth to " << Bchart::Nth << endl;
     }
   if(args.isset('S')) Bchart::silent = true;
   if(args.isset('P')) Bchart::prettyPrint = true;
   if(args.isset('C')) Bchart::caseInsensitive = true;
   if(args.isset('K')) Bchart::tokenize = false;
   if(args.isset('T'))
     {
       int fac = atoi(args.value('T').c_str());
       float ffac = (float)fac;
       ffac /= 10;
       Bchart::timeFactor = ffac;
     }
   if(args.isset('l'))
     {
       maxSentLen = atoi(args.value('l').c_str());
     }
   if( args.isset('d') )
     {
       int lev = atoi(args.value('d').c_str());
       Bchart::printDebug() = lev;
     }
   if( args.isset('L') )Term::Language = args.value('L');

  if( args.isset('n') )
    {
      char etemp[16];
      strcpy(etemp,args.value('n').c_str());
      char *	temp = strchr( etemp, '/' );
      if( !temp )
	error( "No terminal '/' found in '-n' argument" );
      *temp = '\0';
      ofTotal_ = atoi( ++temp );
      char *	mask = new char[ ofTotal_ ];
      for( int i = 0; i < ofTotal_; i++ )
	mask[ i ] = 0;
      // fill in mask with valid numbers;
      ECString tmp2 = etemp;
      numString_ = tmp2;		// meaningful id for this process;
      whichSent_ = atoi(tmp2.c_str());
      mask[ whichSent_ ] = 1;
      field_ = new Field( ofTotal_, mask );
    }
    else
    {
	static char mask[1] = { 1 };
	field_ = new Field( 1, mask );
    }
}
