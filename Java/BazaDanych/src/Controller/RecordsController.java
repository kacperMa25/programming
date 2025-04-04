package Controller;

import Model.*;
import View.RecordsView;

import java.io.*;
import java.util.HashMap;
import java.util.zip.*;

public class RecordsController {
    private HashMap<String, Pracownik> workers;
    private RecordsView recordsView;

    public RecordsController(RecordsView recordsView) {
        this.workers = new HashMap<>();
        this.recordsView = recordsView;
    }

    private void BackUp(){
        String decision = recordsView.createBackUp();
        if (decision.equalsIgnoreCase("Z")) {
            String[] decisions = recordsView.compress();
            if(!recordsView.confirm()) return;
            try {
                if (decisions[0].equals("Z")) {
                    compressZip(decisions[1]);
                } else if (decisions[0].equals("G")) {
                    compressGZip(decisions[1]);
                } else {
                    System.out.println("Invalid decision");
                }
            } catch (IOException e) {
                System.err.println("Błąd podczas tworzenia kopii zapasowej: " + e.getMessage());
            }
        } else if (decision.equalsIgnoreCase("O")) {
            decision = recordsView.decompress();
            if(!recordsView.confirm()) return;
            decompress(decision);
        }
    }

    private void decompress(String fileName) {
        try {
            if (fileName.endsWith(".zip")) {
                readZipBackup(fileName);
            } else if (fileName.endsWith(".gz")) {
                readGzipBackup(fileName);
            } else {
                System.out.println("Nieprawidłowy format pliku. Obsługiwane formaty to: .zip, .gz");
            }
        } catch (IOException | ClassNotFoundException e) {
            System.err.println("Błąd podczas odczytu kopii zapasowej: " + e.getMessage());
        }
    }

    private void compressZip(String fileName) throws IOException {
        try (FileOutputStream fos = new FileOutputStream(fileName);
             ZipOutputStream zos = new ZipOutputStream(fos)) {

            ZipEntry entry = new ZipEntry("workers.dat");
            zos.putNextEntry(entry);

            ObjectOutputStream oos = new ObjectOutputStream(zos);
            oos.writeObject(workers);
            oos.flush();

            zos.closeEntry();
            System.out.println("Kopia zapasowa ZIP została utworzona: " + fileName);
        }
    }


    private void compressGZip(String fileName) throws IOException {
        try (FileOutputStream fos = new FileOutputStream(fileName);
             GZIPOutputStream gzos = new GZIPOutputStream(fos);
             ObjectOutputStream oos = new ObjectOutputStream(gzos)) {

            oos.writeObject(workers);
            System.out.println("Kopia zapasowa GZIP została utworzona: " + fileName);
        }
    }

    private void readZipBackup(String fileName) throws IOException, ClassNotFoundException {
        try (FileInputStream fis = new FileInputStream(fileName);
             ZipInputStream zis = new ZipInputStream(fis)) {

            ZipEntry entry = zis.getNextEntry();
            if (entry != null) {
                try (ObjectInputStream ois = new ObjectInputStream(zis)) {
                    workers = (HashMap<String, Pracownik>) ois.readObject();
                    System.out.println("Dane zostały pomyślnie wczytane z pliku ZIP.");
                }
            } else {
                System.out.println("Nie znaleziono odpowiednich danych w pliku ZIP.");
            }
        }
    }

    private void readGzipBackup(String fileName) throws IOException, ClassNotFoundException {
        try (FileInputStream fis = new FileInputStream(fileName);
             GZIPInputStream gzis = new GZIPInputStream(fis);
             ObjectInputStream ois = new ObjectInputStream(gzis)) {

            workers = (HashMap<String, Pracownik>) ois.readObject();
            System.out.println("Dane zostały pomyślnie wczytane z pliku GZIP.");
        }
    }

    public void addWorker(){
        recordsView.clearConsole();
        Pracownik worker = recordsView.addWorker();
        if (recordsView.confirm()) workers.put(worker.getPesel(), worker);
        recordsView.clearConsole();
    }

    public boolean contaisWorker(String pesel){
        return workers.containsKey(pesel);
    }

    public void removeWorker(){
        Pracownik pracownik = recordsView.removePracownik(workers);
        if ( pracownik != null) {
            if(recordsView.confirm()) workers.remove(pracownik.getPesel());
        }
    }

    public void addWorker(Pracownik worker){
        workers.put(worker.getPesel(), worker);
    }

    public void displayRecords(){
        recordsView.clearConsole();
        recordsView.printWorkers(workers);
        recordsView.clearConsole();
    }

    public boolean displayMenu(){
        recordsView.clearConsole();
        String choice = recordsView.printMenu();

        switch (choice) {
            case "1":
                displayRecords();
                break;
            case "2":
                addWorker();
                break;
            case "3":
                removeWorker();
                break;
            case "4":
                BackUp();
                break;
            case "5":
                return true;
        }
        recordsView.clearConsole();
        return false;
    }
}
