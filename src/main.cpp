#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include <QLocale>
#include <QTranslator>
#include <Qt/qdeclarativedebug.h>
#include "BBLiveGame.hpp"

#include "PaymentServiceControl.hpp"

using namespace bb::cascades;
using namespace bb::platform;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString( "BBLiveGame_%1" ).arg( locale_string );
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator( &translator );
    }

    PaymentManager::setConnectionMode(PaymentConnectionMode::Test);//FOR TESTING ONLY
    qmlRegisterType<PaymentServiceControl>("com.blackberry.payment", 1, 0, "PaymentServiceControl");

    new BBLiveGame(&app);

    return Application::exec();
}
