import Controller.RecordsController;
import View.RecordsView;

public class Main {
    public static void main(String[] args) {
        RecordsView recordsView = new RecordsView();
        RecordsController recordsController = new RecordsController(recordsView);

        while (true) {
            if(recordsController.displayMenu()) break;
        }
    }
}