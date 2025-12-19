#include <QGuiApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QStringList>
#include "WordCloudGenerator.h"

bool isValidShape(const QString &shape) {
    static const QStringList validShapes = {"spiral", "circle", "square", "triangle", "heart", "star"};
   return validShapes.contains(shape.toLower());
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    
    QCoreApplication::setApplicationName("WordCloudGenerator");
    QCoreApplication::setApplicationVersion("1.0");
    
    QCommandLineParser parser;
    parser.setApplicationDescription("Generate word cloud from text file");
    parser.addHelpOption();
    parser.addVersionOption();
    
    parser.addPositionalArgument("input", "Input text file");
    
    parser.addOption(QCommandLineOption({"o", "output"},
        "Output .jpg file", "file", "output.jpg"));
    
    parser.addOption(QCommandLineOption({"s", "shape"}, 
        "Cloud shape: spiral(maximum words: 50), circle(maximum words: 40), square(maximum words: 40), triangle(maximum words: 36), heart(maximum words: 48), star(maximum words: 50)", 
        "shape", "spiral"));
    
    parser.addOption(QCommandLineOption({"W", "width"}, 
        "Image width in pixels (minimum 100)", "pixels", "800"));
    
    parser.addOption(QCommandLineOption({"H", "height"}, 
        "Image height in pixels (minimum 100)", "pixels", "600"));
    
    parser.process(app);
    
    const QStringList args = parser.positionalArguments();

    if (args.isEmpty()) {
        qCritical() << "Error: no text file";
        return 1;
    }
    
    QString inputFile = args.at(0);

    if (!inputFile.endsWith(".txt", Qt::CaseInsensitive)) {
        qCritical() << "Error: Input file must have .txt extension";
        qCritical() << "Got:" << inputFile;
        return 1;
    }

    QString outputFile = parser.value("output");
    QString shapeStr = parser.value("shape");
    
    if (!isValidShape(shapeStr)) {
        qCritical() << "Error: Unknown shape:" << shapeStr;
        qCritical() << "Available shapes: spiral, circle, square, triangle, heart, star";
        return 1;
    }
    
    int width = parser.value("width").toInt();
    int height = parser.value("height").toInt();
    
    if (width < 100 || height < 100) {
        qCritical() << "Error: Minimum image size is 100 by 100 pixels";
        return 1;
    }
    
    QFile file(inputFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Error: Can`t open file" << inputFile;
        return 1;
    }
    
    QString text = QString::fromUtf8(file.readAll());
    file.close();
    
    if (text.isEmpty()) {
        qCritical() << "Error: File is empty";
        return 1;
    }
    
    WordCloudGenerator generator;
    
    generator.setShape(shapeStr);
    
    generator.processText(text);
    
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::white);
    
    QPainter painter(&image);
    generator.draw(&painter, image.size());
    painter.end();
    
    QString outputFileLower = outputFile.toLower();
    
    if (!outputFileLower.endsWith(".jpg") && !outputFileLower.endsWith(".jpeg")) {
        if (!outputFile.contains('.')) {
            outputFile += ".jpg";
        } else {
            int lastDot = outputFile.lastIndexOf('.');
            outputFile = outputFile.left(lastDot) + ".jpg";
        }
        qInfo() << "File`s extension was changed to .jpg:" << outputFile;
    }
    
    if (!image.save(outputFile, "JPG", 100)) {
        qCritical() << "Error: Can`t save an image" << outputFile;
        return 1;
    }
    
    return 0;

}

