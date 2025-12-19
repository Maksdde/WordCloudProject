#ifndef WORDCLOUDGENERATOR_H
#define WORDCLOUDGENERATOR_H

#include <QString>         
#include <QPainter>        
#include <QSize>           
#include <QColor>          
#include <map> 

class WordCloudGenerator {
public:
    void processText(const QString &text);
    void draw(QPainter *p, const QSize &size);
    void setShape(const QString& shape) { drawing_shape = shape.toLower(); }
      
private:
    std::map<QString, int> freq;
    
    QString drawing_shape = "spiral";
    
    static constexpr int MAX_WORDS_SPIRAL = 50;
    static constexpr int MAX_WORDS_CIRCLE = 40;
    static constexpr int MAX_WORDS_SQUARE = 40;
    static constexpr int MAX_WORDS_TRIANGLE = 36;
    static constexpr int MAX_WORDS_HEART = 48;
    static constexpr int MAX_WORDS_STAR = 50;
    static constexpr int MIN_FONT_SIZE = 10;
    static constexpr int MAX_FONT_SIZE = 50;
    static constexpr int BASE_FONT_SIZE_CIRCLE = 14;
    static constexpr int BASE_FONT_SIZE_SPIRAL = 14;
    static constexpr int BASE_FONT_SIZE_SQUARE = 12;
    static constexpr int BASE_FONT_SIZE_TRIANGLE = 12;
    static constexpr int BASE_FONT_SIZE_HEART = 12;
    static constexpr int BASE_FONT_SIZE_STAR = 12;
    static constexpr double CIRCLE_RADIUS_RATIO = 0.33;
    static constexpr double SPIRAL_INNER_RATIO = 0.4;
    static constexpr double SPIRAL_OUTER_RATIO = 0.7;
    
    static const std::vector<QColor> COLORS;
    
    void drawBasic(QPainter *p,
        const QSize &size,
        const std::vector<QPoint>& positions, 
        const QString& fontName = "Arial", 
        int baseFontSize = 12, 
        int fontMultiplier = 18
    );
    
    void drawSpiral(QPainter *p, const QSize &size);
    void drawCircle(QPainter *p, const QSize &size);
    void drawSquare(QPainter *p, const QSize &size); 
    void drawTriangle(QPainter *p, const QSize &size);
    void drawHeart(QPainter *p, const QSize &size);
    void drawStar(QPainter *p, const QSize &size); 
    
    QColor getRandomColor();
};


#endif
