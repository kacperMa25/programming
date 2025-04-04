module com.example.programkadrowyfx {
    requires javafx.controls;
    requires javafx.fxml;


    opens com.example.programkadrowyfx to javafx.fxml;
    exports com.example.programkadrowyfx;
}