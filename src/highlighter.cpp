//LightLang Editor - editor for SL dictionaries
//Copyright (C) 2007-2016 Tikhonov Sergey
//
//This file is part of LightLang Editor
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#include <QtGui>

#include "highlighter.h"

HighLighter::HighLighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
     HighlightingRule rule;

     italicFormat.setFontItalic(true);
     rule.pattern = QRegExp("\\(.*\\)");
     rule.format = italicFormat;
     highlightingRules.append(rule);

     boldFormat.setFontWeight(QFont::Bold);
     rule.pattern = QRegExp("\\[.*\\]");
     rule.format = boldFormat;
     highlightingRules.append(rule);

     underlineFormat.setFontUnderline(true);
     rule.pattern = QRegExp("\\_.*\\_");
     rule.format = underlineFormat;
     highlightingRules.append(rule);
      
     referenceFormat.setForeground(Qt::blue);
     rule.pattern = QRegExp("\\@.*\\@");
     rule.format = referenceFormat;
     highlightingRules.append(rule);
	
     soundFormat.setForeground(Qt::red);
     rule.pattern = QRegExp("\\\\s.*:.*\\\\s");
     rule.format = soundFormat;
     highlightingRules.append(rule);
     
	officeFormat.setFontWeight(QFont::Bold);
	officeFormat.setForeground(Qt::green);
     rule.pattern = QRegExp("\\<.*\\>");
     rule.format = officeFormat;
     highlightingRules.append(rule);	
}

void HighLighter::highlightBlock(const QString& text)
{                                       
	int length = 0;
	int index = 0;
     foreach (HighlightingRule rule, highlightingRules) 
	{
         QRegExp expression(rule.pattern);
         index = text.indexOf(expression);
         while (index >= 0) 
         {
             length = expression.matchedLength();
             setFormat(index+1, length-3, rule.format);
             index = text.indexOf(expression, index + length  );
         }
     }                              
     setCurrentBlockState(0);
}
