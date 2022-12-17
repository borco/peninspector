#include <QTest>

#include "penconfig.h"
#include "pressurehistogrammodel.h"

class TestPressureHistogramModel: public QObject
{
    Q_OBJECT

    typedef QList<int> QIntList;
    typedef QPair<int, int> QIntPair;
    typedef QList<QIntPair> QIntPairList;

private slots:
    void addPressureMinimumSensitivity() {
        // 8192 is the maximum currently available
        // we verify we can detect 8x more levels
        const int levels = 8192 * 8;

        const qreal delta = 1.0 / levels;

        PenConfig config;
        config.setPressureLevels(levels);
        PressureHistogramModel model(&config);

        for (int i = 0; i <= levels; ++i) {
            model.addPressure(i * delta);
        }

        QCOMPARE(model.size(), levels);
    }

    void addPressureLevelAndCount_data() {
        QTest::addColumn<int>("levels");
        QTest::addColumn<QIntList>("values");
        QTest::addColumn<QIntPairList>("histogram");

        QTest::newRow("") << 8192 * 8 << (QIntList() << 0 << 2) << (QIntPairList() << QIntPair{2, 1});
        QTest::newRow("") << 8192 * 8
                          << (QIntList() << 0 << 2 << 3 << 1 << 2)
                          << (QIntPairList() << QIntPair{1, 1} << QIntPair{2, 2} << QIntPair{3, 1});
    }

    void addPressureLevelAndCount() {
        QFETCH(int, levels);
        QFETCH(QIntList, values);
        QFETCH(QIntPairList, histogram);

        PenConfig config;
        config.setPressureLevels(levels);
        PressureHistogramModel model(&config);

        const qreal delta = 1.0 / levels;

        for (int value: values) {
            model.addPressure(value * delta);
        }

        QCOMPARE(model.size(), histogram.size());

        for (int i = 0; i < model.size(); ++i) {
            int level = histogram[i].first;
            int count = histogram[i].second;
            qreal value = delta * level;

            QCOMPARE(model[i].value, value);
            QCOMPARE(model[i].level, level);
            QCOMPARE(model[i].count, count);
        }
    }
};

QTEST_MAIN(TestPressureHistogramModel)
#include "test_pressurehistogrammodel.moc"
