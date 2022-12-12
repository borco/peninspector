/*
    Copyright 2022 by Ioan Calin Borcoman <iborco@gmail.com>

    This file is part of PenInspector.

    PenInspector is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    PenInspector is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    PenInspector. If not, see <https://www.gnu.org/licenses/>.
*/

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
