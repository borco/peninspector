#pragma once

#include <QWidget>

class PenConfig;
class PenInfo;

class TiltPreview : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(PenConfig* penConfig READ penConfig WRITE setPenConfig NOTIFY penConfigChanged)
    Q_PROPERTY(PenInfo* penInfo READ penInfo WRITE setPenInfo NOTIFY penInfoChanged)

public:
    explicit TiltPreview(QWidget *parent = nullptr);

    PenConfig *penConfig() const { return m_penConfig; }
    void setPenConfig(PenConfig *newPenConfig);

    PenInfo *penInfo() const { return m_penInfo; }
    void setPenInfo(PenInfo *newPenInfo);

signals:
    void penConfigChanged();
    void penInfoChanged();

protected:
     void paintEvent(QPaintEvent *event) override;

private:
     PenConfig *m_penConfig = nullptr;
     PenInfo *m_penInfo = nullptr;
};

