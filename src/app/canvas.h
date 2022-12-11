#pragma once

#include <QWidget>
#include <QImage>

class PenInfo;

class Canvas : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int brushSize READ brushSize WRITE setBrushSize NOTIFY brushSizeChanged)

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    void clear();

    int brushSize() const { return m_brushSize; }
    void setBrushSize(int newBrushSize);

    PenInfo* penInfo() const { return m_penInfo; }

signals:
    void brushSizeChanged();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
    void tabletEvent(QTabletEvent *event) override;

private:
    void paintAt(QPointF point);
    void updatePenInfo(QTabletEvent *event) const;

    QImage m_image;
    int m_brushSize {30};
    PenInfo* m_penInfo {nullptr};
};
