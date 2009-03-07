#include "HighLighter.h"

HighLighter::HighLighter(QTextDocument *parent): QSyntaxHighlighter(parent) {
	HighlightingRule rule;
	
	QTextCharFormat boldFormat;
	boldFormat.setFontWeight(QFont::Bold);
	
	QTextCharFormat underlineFormat;
	underlineFormat.setForeground(Qt::green);
	
	QTextCharFormat referenceFormat;
	referenceFormat.setForeground(Qt::blue);
	
	QTextCharFormat soundFormat;
	soundFormat.setForeground(Qt::gray);
	
	QTextCharFormat officeFormat;
	officeFormat.setForeground(Qt::darkGreen);
	
	rule.pattern = QRegExp("\\\\\\(");
	rule.format = boldFormat;
	highlightingRules.append(rule);
	
	rule.pattern = QRegExp("\\\\\\)");
	rule.format = boldFormat;
	highlightingRules.append(rule);

	rule.pattern = QRegExp("\\\\\\[");
	rule.format = boldFormat;
	highlightingRules.append(rule);
	
	rule.pattern = QRegExp("\\\\\\]");
	rule.format = boldFormat;
	highlightingRules.append(rule);

	rule.pattern = QRegExp("\\\\_");
	rule.format = underlineFormat;
	highlightingRules.append(rule);
	
	rule.pattern = QRegExp("\\\\@");
	rule.format = referenceFormat;
	highlightingRules.append(rule);

	rule.pattern = QRegExp("\\\\s");
	rule.format = soundFormat;
	highlightingRules.append(rule);
     
	rule.pattern = QRegExp("\\\\<");
	rule.format = officeFormat;
	highlightingRules.append(rule);	
	
	rule.pattern = QRegExp("\\\\>");
	rule.format = officeFormat;
	highlightingRules.append(rule);	
}

void HighLighter::highlightBlock(const QString& text) {                         
	int index = 0;
	foreach (HighlightingRule rule, highlightingRules) 
	{
         QRegExp expression(rule.pattern);
         index = text.indexOf(expression);
         while (index >= 0) 
         {
			setFormat(index, 2, rule.format);
            index = text.indexOf(expression, index + 1);
         }
     }                              
     setCurrentBlockState(0);
}
