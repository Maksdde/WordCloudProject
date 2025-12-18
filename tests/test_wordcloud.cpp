#include <gtest/gtest.h>
#include "../WordCloudGenerator.h"
#include <QImage>
#include <QPainter>
#include <QGuiApplication>

TEST(WordCloudTest, DifferentShapes) {  // отрисовка разных форм
    int argc = 1;
    int i = 1;
    char arg0[] = "test";
    char* argv[] = {arg0, nullptr};
    QGuiApplication app(argc, argv);
    
    WordCloudGenerator generator;
    generator.processText("one two three four five six seven eight nine ten");
    
    const QStringList shapes = {"spiral", "circle", "square", "triangle", "heart", "star"};
    
    for (const auto& shape : shapes) {
        generator.setShape(shape);
        
        QImage image(i * 150, i * 100, QImage::Format_ARGB32);
        image.fill(Qt::white);
        QPainter painter(&image);
        
        EXPECT_NO_THROW(generator.draw(&painter, image.size()));
        
        image.save(QString("test_%1.jpg").arg(shape));
        i += 1;
    }
}

TEST(WordCloudTest, Empty) {  // подача пустого текста
    int argc = 1;
    char arg0[] = "test";
    char* argv[] = {arg0, nullptr};
    QGuiApplication app(argc, argv);
    
    WordCloudGenerator generator;
    generator.processText("");
    generator.setShape("circle");
    QImage image(800, 600, QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    
    EXPECT_NO_THROW(generator.draw(&painter, image.size()));
    
    QColor centerColor = image.pixelColor(400, 300);
    EXPECT_EQ(centerColor, Qt::white);
    image.save(QString("test_empty.jpg"));
}

TEST(WordCloudTest, WrongShape) {  // задаём отсутствющую форму
    int argc = 1;
    char arg0[] = "test";
    char* argv[] = {arg0, nullptr};
    QGuiApplication app(argc, argv);
    WordCloudGenerator generator;
    generator.processText("one two three four five six seven eight nine ten eleven twelve thirteen");

    generator.setShape("boogy-woogy");

    QImage image(800, 600, QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    
    EXPECT_NO_THROW(generator.draw(&painter, image.size()));
    image.save(QString("test_boogy.jpg"));
}
