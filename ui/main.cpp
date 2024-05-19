#include <QApplication>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <chrono>
#include <filesystem>
#include <iostream>

#include "../src/csvLoader.hpp"
#include "../src/database.hpp"

int main(int argc, char *argv[]) {
    // ------------------------------------------------------------------------

    std::cout << std::filesystem::current_path() << std::endl;

    Database *db = new Database();

    // ------------------------------------------------------------------------
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("DBII Proy1");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Top layout for help button
    QHBoxLayout *topLayout = new QHBoxLayout;

    // Execution time label
    QLabel *executionTimeLabel = new QLabel("Execution time: ...");
    topLayout->addWidget(executionTimeLabel);

    // Spacer to push the help button to the right
    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    topLayout->addItem(spacer);

    // ------------------------------------------------------------------------
    // Help button
    QPushButton *helpButton = new QPushButton;
    helpButton->setText("?");
    helpButton->setFixedSize(30, 30);
    topLayout->addWidget(helpButton);
    // ------------------------------------------------------------------------

    mainLayout->addLayout(topLayout);

    QLineEdit *queryInput = new QLineEdit;
    queryInput->setPlaceholderText("Enter SQL query...");
    mainLayout->addWidget(queryInput);

    QPushButton *executeButton = new QPushButton("Execute");
    mainLayout->addWidget(executeButton);

    QTableWidget *resultTable = new QTableWidget;
    resultTable->setColumnCount(10);  // Set the number of columns based on your needs
    resultTable->setHorizontalHeaderLabels({"CustomerID", "ProductID", "Quantity", "Price", "TransactionDate",
                                            "PaymentMethod", "StoreLocation", "ProductCategory", "DiscountApplied",
                                            "TotalAmount"});
    mainLayout->addWidget(resultTable);

    auto executeQuery = [=]() {
        resultTable->clearContents();
        // Print the query input to the console
        QString query = queryInput->text();
        std::cout << "Query: " << query.toStdString() << std::endl;

        auto start = std::chrono::high_resolution_clock::now();

        std::cout << db->execute(query.toStdString()) << std::endl;

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        // Update the execution time label
        executionTimeLabel->setText("Execution time: " + QString::number(elapsed.count()) + " seconds");

        resultTable->setRowCount(db->last_results.size());
        for (int i = 0; i < db->last_results.size(); i++) {
            Register reg = db->last_results[i];
            resultTable->setItem(i, 0, new QTableWidgetItem(QString::number(reg.CustomerID)));
            resultTable->setItem(i, 1, new QTableWidgetItem(QString(reg.ProductID)));
            resultTable->setItem(i, 2, new QTableWidgetItem(QString::number(reg.Quantity)));
            resultTable->setItem(i, 3, new QTableWidgetItem(QString::number(reg.Price)));
            resultTable->setItem(i, 4, new QTableWidgetItem(QString(reg.TransactionDate)));
            resultTable->setItem(i, 5, new QTableWidgetItem(QString(reg.PaymentMethod)));
            resultTable->setItem(i, 6, new QTableWidgetItem(QString(reg.StoreLocation)));
            resultTable->setItem(i, 7, new QTableWidgetItem(QString(reg.ProductCategory)));
            resultTable->setItem(i, 8, new QTableWidgetItem(QString::number(reg.DiscountApplied)));
            resultTable->setItem(i, 9, new QTableWidgetItem(QString::number(reg.TotalAmount)));
        }
    };

    QObject::connect(executeButton, &QPushButton::clicked, executeQuery);
    QObject::connect(queryInput, &QLineEdit::returnPressed, executeQuery);

    // ------------------------------------------------------------------------
    QObject::connect(helpButton, &QPushButton::clicked, [&]() {
        QMessageBox::information(&window, "Help",
                                 "Create table:\n"
                                 "CREATE TABLE Customer FROM FILE dataA.csv USING INDEX"
                                 "AVL | HASH | ISAM\n\n"
                                 "Select:\n"
                                 "SELECT * FROM Customer WHERE CustomerID = x\n"
                                 "SELECT * FROM Customer WHERE CustomerID BETWEEN x AND y\n\n"
                                 "Insert:\n"
                                 "INSERT INTO Customer VALUES (x,y,z,...)\n\n"
                                 "Delete:\n"
                                 "DELETE FROM Customer WHERE CustomerID = x");
    });
    // ------------------------------------------------------------------------

    window.setLayout(mainLayout);
    window.resize(600, 450);
    window.show();

    return app.exec();
}
