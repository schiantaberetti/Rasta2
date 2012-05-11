//========================================================================
//
// pdftotext.cc
//
// Copyright 1997-2003 Glyph & Cog, LLC
//
// Modified for Debian by Hamish Moffatt, 22 May 2002.
//
//========================================================================

//========================================================================
//
// Modified under the Poppler project - http://poppler.freedesktop.org
//
// All changes made under the Poppler project to this file are licensed
// under GPL version 2 or later
//
// Copyright (C) 2006 Dominic Lachowicz <cinamod@hotmail.com>
// Copyright (C) 2007-2008, 2010, 2011 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2009 Jan Jockusch <jan@jockusch.de>
// Copyright (C) 2010 Hib Eris <hib@hiberis.nl>
// Copyright (C) 2010 Kenneth Berland <ken@hero.com>
// Copyright (C) 2011 Tom Gleason <tom@buildadam.com>
// Copyright (C) 2011 Steven Murdoch <Steven.Murdoch@cl.cam.ac.uk>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================
#ifndef _pdftotext_h_
#define _pdftotext_h_ 1

#include <poppler-config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "goo/GooString.h"
#include "goo/gmem.h"
#include "GlobalParams.h"
#include "Object.h"
#include "Stream.h"
#include "Array.h"
#include "Dict.h"
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"
#include "PDFDoc.h"
#include "PDFDocFactory.h"
#include "TextOutputDev.h"
#include "CharTypes.h"
#include "UnicodeMap.h"
#include "PDFDocEncoding.h"
#include "Error.h"
#include <string>

static void printInfoString(FILE *f, Dict *infoDict, char *key,
			    char *text1, char *text2, UnicodeMap *uMap);
static void printInfoDate(FILE *f, Dict *infoDict, char *key, char *fmt);

static int firstPage = 1;
static int lastPage = 0;
static double resolution = 72.0;
static int x = 0;
static int y = 0;
static int w = 0;
static int h = 0;
static GBool bbox = gFalse;
static GBool physLayout = gFalse;
static GBool rawOrder = gFalse;
static GBool htmlMeta = gFalse;
static char textEncName[128] = "";
static char textEOL[16] = "";
static GBool noPageBreaks = gFalse;
static char ownerPassword[33] = "\001";
static char userPassword[33] = "\001";
static GBool quiet = gFalse;
static GBool printVersion = gFalse;
static GBool printHelp = gFalse;
static GBool printEnc = gFalse;

static std::string myStringReplace(const std::string &inString, const std::string &oldToken, const std::string &newToken) {
  std::string result = inString;
  size_t foundLoc;
  int advance = 0;
  do {
    foundLoc = result.find(oldToken, advance);
    if (foundLoc != std::string::npos){
      result.replace(foundLoc, oldToken.length(), newToken);
      advance = foundLoc + newToken.length();
    }
  } while (foundLoc != std::string::npos );
  return result;
}

static std::string myXmlTokenReplace(const char *inString){
  std::string myString(inString);
  myString = myStringReplace(myString, "&",  "&amp;" );
  myString = myStringReplace(myString, "'",  "&apos;" );
  myString = myStringReplace(myString, "\"", "&quot;" );
  myString = myStringReplace(myString, "<",  "&lt;" );
  myString = myStringReplace(myString, ">",  "&gt;" );
  return myString;
}
static void printInfoString(FILE *f, Dict *infoDict, char *key,
			    char *text1, char *text2, UnicodeMap *uMap) {
  Object obj;
  GooString *s1;
  GBool isUnicode;
  Unicode u;
  char buf[8];
  int i, n;

  if (infoDict->lookup(key, &obj)->isString()) {
    fputs(text1, f);
    s1 = obj.getString();
    if ((s1->getChar(0) & 0xff) == 0xfe &&
	(s1->getChar(1) & 0xff) == 0xff) {
      isUnicode = gTrue;
      i = 2;
    } else {
      isUnicode = gFalse;
      i = 0;
    }
    while (i < obj.getString()->getLength()) {
      if (isUnicode) {
	u = ((s1->getChar(i) & 0xff) << 8) |
	    (s1->getChar(i+1) & 0xff);
	i += 2;
      } else {
	u = pdfDocEncoding[s1->getChar(i) & 0xff];
	++i;
      }
      n = uMap->mapUnicode(u, buf, sizeof(buf));
      fwrite(buf, 1, n, f);
    }
    fputs(text2, f);
  }
  obj.free();
}

static void printInfoDate(FILE *f, Dict *infoDict, char *key, char *fmt) {
  Object obj;
  char *s;

  if (infoDict->lookup(key, &obj)->isString()) {
    s = obj.getString()->getCString();
    if (s[0] == 'D' && s[1] == ':') {
      s += 2;
    }
    fprintf(f, fmt, s);
  }
  obj.free();
}
#endif
