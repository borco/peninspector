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
    void addPressureMinimumSensitivity_data() {
        QTest::addColumn<int>("pressureLevels");
        QTest::addColumn<int>("windowSize");

        QTest::newRow("levels: 8192 * 8, window: 2") << 8192 * 8 << 2;
        QTest::newRow("levels: 8192 * 8, window: 20") << 8192 * 8 << 20;
        QTest::newRow("levels: 8192 * 8, window: 200") << 8192 * 8 << 200;
        QTest::newRow("levels: 8192 * 8, window: -1") << 8192 * 8 << -1;
    }

    void addPressureMinimumSensitivity() {
        // 8192 is the maximum currently available
        // we verify we can detect 8x more levels
        const int levels = 8192 * 8;
        const int window_size = 20;
        const qreal delta = 1.0 / levels;

        PenConfig config;
        config.setPressureLevels(levels);
        PressureHistogramModel model(&config);
        model.setWindowSize(window_size);

        for (int i = 0; i <= levels; ++i) {
            model.addPressure(i * delta);
        }

        QCOMPARE(model.totalLevels(), levels);
    }

    void addPressureLevelAndCount_data() {
        QTest::addColumn<int>("pressureLevels");
        QTest::addColumn<int>("windowSize");
        QTest::addColumn<QIntList>("levels");
        QTest::addColumn<QIntPairList>("histogram");

        QTest::newRow("window: -1 | levels: 0, 2")
                << 8192 * 8
                << -1
                << (QIntList() << 0 << 2)
                << (QIntPairList() << QIntPair{2, 1});

        QTest::newRow("window: -1 | levels: 0, 3, 2, 1, 2")
                << 8192 * 8
                << -1
                << (QIntList() << 0 << 3 << 2 << 1 << 2)
                << (QIntPairList() << QIntPair{1, 1} << QIntPair{2, 2} << QIntPair{3, 1});

        QTest::newRow("window: 2 | levels: 0, 3, 2, 1, 2")
                << 8192 * 8
                << 2
                << (QIntList() << 0 << 3 << 2 << 1 << 2)
                << (QIntPairList() << QIntPair{1, 1} << QIntPair{2, 1});

        QTest::newRow("window: 3 | levels: 0, 3, 2, 1, 2")
                << 8192 * 8
                << 3
                << (QIntList() << 0 << 3 << 2 << 1 << 2)
                << (QIntPairList() << QIntPair{1, 1} << QIntPair{2, 2});

        QTest::newRow("window: 2 | levels: 0, 3, 2, 1, 2, 3, 3, 3")
                << 8192 * 8
                << 3
                << (QIntList() << 0 << 3 << 2 << 1 << 2 << 3 << 3 << 3)
                << (QIntPairList() << QIntPair{3, 3});
    }

    void addPressureLevelAndCount() {
        QFETCH(int, pressureLevels);
        QFETCH(int, windowSize);
        QFETCH(QIntList, levels);
        QFETCH(QIntPairList, histogram);

        PenConfig config;
        config.setPressureLevels(pressureLevels);
        PressureHistogramModel model(&config);
        model.setWindowSize(windowSize);

        const qreal delta = 1.0 / pressureLevels;

        for (int level: levels) {
            model.addPressure(level * delta);
        }

        QCOMPARE(model.histogramSize(), histogram.size());

        for (int i = 0; i < model.histogramSize(); ++i) {
            int level = histogram[i].first;
            int count = histogram[i].second;
            qreal value = level * delta;

            QCOMPARE(model[i].level, level);
            QCOMPARE(model[i].value, value);
            QCOMPARE(model[i].count, count);
        }
    }
};

QTEST_MAIN(TestPressureHistogramModel)
#include "test_pressurehistogrammodel.moc"
