#ifndef SPEED_TEST_FACTORY_METHOD_H
#define SPEED_TEST_FACTORY_METHOD_H

#include <QString>

// forward declaration
class SpeedTestParser;

SpeedTestParser* createParser(QString type);

#endif // SPEED_TEST_FACTORY_METHOD_H
