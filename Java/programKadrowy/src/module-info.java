module com.example.programkadrowy {
    requires javafx.controls;
    requires javafx.fxml;


    opens com.example.programkadrowy to javafx.fxml;
    exports com.example.programkadrowy;
}