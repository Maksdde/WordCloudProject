#include "WordCloudGenerator.h"
#include <QStringList>
#include <QPainter>
#include <QRegularExpression>
#include <QDateTime>
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <QFont>           
#include <QFontMetrics>    
#include <QRandomGenerator>
#include <vector>

const std::vector<QColor> WordCloudGenerator::COLORS = {  // цвета
    QColor(231, 76, 60),    // красный
    QColor(46, 204, 113),   // зеленый
    QColor(52, 152, 219),   // синий
    QColor(155, 89, 182),   // фиолетовый
    QColor(241, 196, 15),   // жёлтый
    QColor(230, 126, 34),   // оранжевый
    QColor(22, 160, 133)    // бирюзовый
};

void WordCloudGenerator::processText(const QString &text) {  // считывание и уборка шума в тексте
    freq.clear();
    
    QStringList words = text.toLower().split(QRegularExpression("[^a-zа-яё0-9]+"), Qt::SkipEmptyParts);
    
    for (auto &w : words) {
        if (w.length() < 2) continue;
        freq[w]++;
    }
}

void WordCloudGenerator::draw(QPainter *p, const QSize &size) {  // выбор фигуры
    if (freq.empty()) return;
    if (drawing_shape == "circle") {
        drawCircle(p, size);
    } else if (drawing_shape == "spiral") {
        drawSpiral(p, size);
    } else if (drawing_shape == "square") {
        drawSquare(p, size);
    } else if (drawing_shape == "triangle") {
        drawTriangle(p, size);
    } else if (drawing_shape == "heart") {
        drawHeart(p, size);
    } else if (drawing_shape == "star") {
        drawStar(p, size);
    } else {
        drawSpiral(p, size);  // по умолчанию спираль
    }
}

QColor WordCloudGenerator::getRandomColor() {  // выбор случайного цвета
    return COLORS[QRandomGenerator::global()->bounded(COLORS.size())];
}

void WordCloudGenerator::drawBasic(QPainter *p, const QSize &size, // общий метод рисования
                                    const std::vector<QPoint> &positions,
                                    const QString &fontName,
                                    int baseFontSize,
                                    int fontMultiplier) {
    if (freq.empty() || positions.empty()) return;
    
    std::vector<std::pair<QString, int>> sortedWords(freq.begin(), freq.end());
    std::sort(sortedWords.begin(), sortedWords.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
    
    int maxWords = std::min(static_cast<int>(sortedWords.size()), static_cast<int>(positions.size()));
    
    for (int i = 0; i < maxWords; i++) {
        const auto &word = sortedWords[i].first;
        int frequency = sortedWords[i].second;
        
        int fontSize = baseFontSize;
        if (sortedWords[0].second > 0) {
            fontSize = baseFontSize + (frequency * fontMultiplier) / (sortedWords[0].second + 1);
        }
        fontSize = std::max(MIN_FONT_SIZE, std::min(MAX_FONT_SIZE, fontSize));
        
        QFont font(fontName, fontSize, QFont::Bold);
        QFontMetrics fm(font);
        int wordWidth = fm.horizontalAdvance(word);
        
        int x = positions[i].x() - wordWidth / 2;
        int y = positions[i].y() + fontSize / 3;
        
        x = std::max(10, std::min(x, size.width() - wordWidth - 10));
        y = std::max(fontSize, std::min(y, size.height() - 10));
        
        QColor color = getRandomColor();
        
        p->setPen(QColor(0, 0, 0, 80));
        p->setFont(font);
        p->drawText(x + 1, y + 1, word);
        p->setPen(color);
        p->drawText(x, y, word);
    }
}

void WordCloudGenerator::drawSpiral(QPainter *p, const QSize &size) {  // спираль
    if (freq.empty()) return;
    
    int maxWords = std::min(static_cast<int>(freq.size()), MAX_WORDS_SPIRAL);
    
    std::vector<QPoint> positions;
    int centerX = size.width() / 2;
    int centerY = size.height() / 2;
    int radius = std::min(size.width(), size.height()) * CIRCLE_RADIUS_RATIO;
    
    for (int i = 0; i < maxWords; i++) {
        double angle = 4 * M_PI * i / maxWords;
        double r = radius * (SPIRAL_INNER_RATIO + SPIRAL_OUTER_RATIO * (i / double(maxWords)));
        
        int x = centerX + static_cast<int>(r * cos(angle));
        int y = centerY + static_cast<int>(r * sin(angle));
        positions.push_back(QPoint(x, y));
    }
    
    drawBasic(p, size, positions, "Arial", BASE_FONT_SIZE_SPIRAL, 16);
}

void WordCloudGenerator::drawCircle(QPainter *p, const QSize &size) {  // круг
    if (freq.empty()) return;
    
    int maxWords = std::min(static_cast<int>(freq.size()), MAX_WORDS_CIRCLE);
    
    std::vector<QPoint> positions;
    int centerX = size.width() / 2;
    int centerY = size.height() / 2;
    int radius = std::min(size.width(), size.height()) * CIRCLE_RADIUS_RATIO;
    
    for (int i = 0; i < maxWords; i++) {
        double angle = 2 * M_PI * i / maxWords;
        int x = centerX + static_cast<int>(radius * cos(angle));
        int y = centerY + static_cast<int>(radius * sin(angle));
        positions.push_back(QPoint(x, y));
    }
    
    drawBasic(p, size, positions, "Arial", BASE_FONT_SIZE_CIRCLE, 18);
}

void WordCloudGenerator::drawSquare(QPainter *p, const QSize &size) {  // квадрат
    if (freq.empty()) return;
    
    int maxWords = std::min(static_cast<int>(freq.size()), MAX_WORDS_SQUARE);

    std::vector<QPoint> positions;
    int length = std::min(size.width(), size.height()) * 2 / 3;
    int startX = (size.width() - length) / 2;
    int startY = (size.height() - length) / 2;
    int endX = startX + length;
    int endY = startY + length;
    
    double perimeter = 4.0 * length;
    double pause = perimeter / maxWords;
    
    for (int i = 0; i < maxWords; i++) {
        double pos = i * pause;
        int x, y;
        
        if (pos < length) {
            x = startX + static_cast<int>(pos);
            y = startY;
        } else if (pos < 2 * length) {
            x = endX;
            y = startY + static_cast<int>(pos - length);
        } else if (pos < 3 * length) {
            x = endX - static_cast<int>(pos - 2 * length);
            y = endY;
        } else {
            x = startX;
            y = endY - static_cast<int>(pos - 3 * length);
        }
        
        positions.push_back(QPoint(x, y));
    }

    drawBasic(p, size, positions, "Arial", BASE_FONT_SIZE_SQUARE, 20);
}

void WordCloudGenerator::drawTriangle(QPainter *p, const QSize &size) {  // треугольник
    if (freq.empty()) return;

    int maxWords = std::min(static_cast<int>(freq.size()), MAX_WORDS_TRIANGLE);
    
    std::vector<QPoint> positions;
    int centerX = size.width() / 2;
    int centerY = size.height() / 2;
    int height = std::min(size.width(), size.height()) / 2;
    int base = height * 1.5;
    
    int topX = centerX;
    int topY = centerY - height / 2;
    int leftX = centerX - base / 2;
    int leftY = centerY + height / 2;
    int rightX = centerX + base / 2;
    int rightY = centerY + height / 2;

    double sideRight = sqrt(pow(rightX - topX, 2) + pow(rightY - topY, 2));
    double sideLeft = sqrt(pow(topX - leftX, 2) + pow(topY - leftY, 2));
    double sideBase = rightX - leftX;
    
    double perimeter = sideRight + sideLeft + sideBase;
    double pause = perimeter / maxWords;
    
    for (int i = 0; i < maxWords; i++) {
        double pos = i * pause;
        int x, y;
        
        if (pos < sideRight) {
            double ratio = pos / sideRight;
            x = static_cast<int>(topX + (rightX - topX) * ratio);
            y = static_cast<int>(topY + (rightY - topY) * ratio);
        } else if (pos < sideRight + sideBase) {
            double ratio = (pos - sideRight) / sideBase;
            x = static_cast<int>(rightX - (rightX - leftX) * ratio);
            y = rightY;
        } else {
            double ratio = (pos - sideRight - sideBase) / sideLeft;
            x = static_cast<int>(leftX + (topX - leftX) * ratio);
            y = static_cast<int>(leftY + (topY - leftY) * ratio);
        }
        
        positions.push_back(QPoint(x, y));
    }
    
    drawBasic(p, size, positions, "Arial", BASE_FONT_SIZE_TRIANGLE, 20);
}

void WordCloudGenerator::drawHeart(QPainter *p, const QSize &size) {  // сердце
    if (freq.empty()) return;

    int maxWords = std::min(static_cast<int>(freq.size()), MAX_WORDS_HEART);
    
    std::vector<QPoint> positions;
    int centerX = size.width() / 2;
    int centerY = size.height() / 2
    int heartSize = std::min(size.width(), size.height()) / 3;
    
    std::vector<QPoint> heartPoints;
    int numPoints = 200;
    
    for (int i = 0; i < numPoints; i++) {
        double t = 2.0 * M_PI * i / numPoints;
        
        double x = 16.0 * pow(sin(t), 3.0);
        double y = 13.0 * cos(t) - 5.0 * cos(2.0*t) - 2.0 * cos(3.0*t) - cos(4.0*t);
        
        int px = centerX + static_cast<int>((x * heartSize) / 17.0);
        int py = centerY - static_cast<int>((y * heartSize) / 17.0);
        
        heartPoints.push_back(QPoint(px, py));
    }
    
    heartPoints.push_back(heartPoints[0]);
    std::rotate(heartPoints.begin(), heartPoints.begin() + heartPoints.size() / 4, heartPoints.end());
    
    double perimeter = 0.0;
    for (size_t i = 1; i < heartPoints.size(); i++) {
        double dx = heartPoints[i].x() - heartPoints[i-1].x();
        double dy = heartPoints[i].y() - heartPoints[i-1].y();
        perimeter += sqrt(dx*dx + dy*dy);
    }
    
    double pause = perimeter / maxWords;
    
    for (int i = 0; i < maxWords; i++) {
        double pos = i * pause;
        double distance = 0.0;
        QPoint point;
        
        for (size_t j = 1; j < heartPoints.size(); j++) {
            double dx = heartPoints[j].x() - heartPoints[j-1].x();
            double dy = heartPoints[j].y() - heartPoints[j-1].y();
            double segmentLength = sqrt(dx*dx + dy*dy);
            
            if (distance + segmentLength >= pos) {
                double ratio = (pos - distance) / segmentLength;
                int px = heartPoints[j-1].x() + static_cast<int>(dx * ratio);
                int py = heartPoints[j-1].y() + static_cast<int>(dy * ratio);
                point = QPoint(px, py);
                break;
            }
            distance += segmentLength;
        }
        
        positions.push_back(point);
    }
    
    drawBasic(p, size, positions, "Arial", BASE_FONT_SIZE_HEART, 18);
}

void WordCloudGenerator::drawStar(QPainter *p, const QSize &size) {  // звезда
    if (freq.empty()) return;
    
    int maxWords = std::min(static_cast<int>(freq.size()), MAX_WORDS_STAR);
    
    std::vector<QPoint> positions;
    int centerX = size.width() / 2;
    int centerY = size.height() / 2;
    int outerRadius = std::min(size.width(), size.height()) / 3;
    int innerRadius = outerRadius / 2;
    
    std::vector<QPoint> starPoints;
    int numPoints = 10;
    
    for (int i = 0; i < numPoints; i++) {
        double angle = 2.0 * M_PI * i / numPoints - M_PI / 2.0;
        int radius = (i % 2 == 0) ? outerRadius : innerRadius;
        
        int x = centerX + static_cast<int>(radius * cos(angle));
        int y = centerY + static_cast<int>(radius * sin(angle));
        
        starPoints.push_back(QPoint(x, y));
    }
    
    starPoints.push_back(starPoints[0]);
    
    double perimeter = 0.0;
    for (size_t i = 1; i < starPoints.size(); i++) {
        double dx = starPoints[i].x() - starPoints[i-1].x();
        double dy = starPoints[i].y() - starPoints[i-1].y();
        perimeter += sqrt(dx*dx + dy*dy);
    }
    
    double pause = perimeter / maxWords;
    
    for (int i = 0; i < maxWords; i++) {
        double pos = i * pause;
        double accumulated = 0.0;
        QPoint point;
        
        for (size_t j = 1; j < starPoints.size(); j++) {
            double dx = starPoints[j].x() - starPoints[j-1].x();
            double dy = starPoints[j].y() - starPoints[j-1].y();
            double segmentLength = sqrt(dx*dx + dy*dy);
            
            if (accumulated + segmentLength >= pos) {
                double ratio = (pos - accumulated) / segmentLength;
                int px = starPoints[j-1].x() + static_cast<int>(dx * ratio);
                int py = starPoints[j-1].y() + static_cast<int>(dy * ratio);
                point = QPoint(px, py);
                break;
            }
            accumulated += segmentLength;
        }
        
        positions.push_back(point);
    }
    
    drawBasic(p, size, positions, "Arial", BASE_FONT_SIZE_STAR, 18);
}


