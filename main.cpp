#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "main_controller.h"

template <typename T>
QObject *obj_instance(T& object, QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    engine->setObjectOwnership(&object, QQmlEngine::CppOwnership);
    return &object;
}

QObject *main_instance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    static MainController main_controller;
    return obj_instance(main_controller, engine, scriptEngine);
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<MainController>("App", 1, 0, "MainController",
                                             &main_instance);
    QQuickView view;
    view.rootContext()->setContextProperty("countsModel",
                                           QVariant::fromValue(&MainController::model()));
    view.setSource(QUrl(QStringLiteral("qrc:/Main.qml")));
    QObject::connect(view.engine(), &QQmlEngine::quit, [](){
        QGuiApplication::quit();
    });

    view.show();
    return app.exec();
}
