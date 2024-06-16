#include "highlighterQss.h"


HighlighterQSS::HighlighterQSS(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Keywords
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        QStringLiteral("\\bcolor\\b"), QStringLiteral("\\bbackground-color\\b"), QStringLiteral("\\bfont-size\\b"),
        QStringLiteral("\\bfont-weight\\b"), QStringLiteral("\\bfont-style\\b"), QStringLiteral("\\bfont-family\\b"),
        QStringLiteral("\\bmargin\\b"), QStringLiteral("\\bpadding\\b"), QStringLiteral("\\bborder\\b"),
        QStringLiteral("\\bwidth\\b"), QStringLiteral("\\bheight\\b"), QStringLiteral("\\bmin-width\\b"),
        QStringLiteral("\\bmax-width\\b"), QStringLiteral("\\bmin-height\\b"), QStringLiteral("\\bmax-height\\b"),
        QStringLiteral("\\bqproperty-\\w+\\b"), QStringLiteral("\\bqss-\\w+\\b"), QStringLiteral("\\bspacing\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Class
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\.[A-Za-z_][A-Za-z0-9_-]*\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    // ID
    idFormat.setFontWeight(QFont::Bold);
    idFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegularExpression(QStringLiteral("#[A-Za-z_][A-Za-z0-9_-]*\\b"));
    rule.format = idFormat;
    highlightingRules.append(rule);



    // Comments
    multiLineCommentFormat.setForeground(Qt::red);

    // Quotation
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Numbers
    numberFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegularExpression(QStringLiteral("\\b\\d+(\\.\\d+)?\\b"));
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Function calls (rgba(), url())
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z_][A-Za-z0-9_-]*\\s*\\("));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Multi-line comment
    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void HighlighterQSS::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
