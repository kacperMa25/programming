package View;

import Model.*;
import java.util.HashMap;
import java.util.Scanner;

public class RecordsView {
    Scanner scanner = new Scanner(System.in);
    public String printMenu(){
        System.out.println("Menu");
        System.out.println("\t 1. Lista pracowników");
        System.out.println("\t 2. Dodaj pracownika");
        System.out.println("\t 3. Usuń pracownika");
        System.out.println("\t 4. Kopia zapasowa");
        System.out.println("\t 5. Wyjdź z programu\n");
        System.out.print("Wybór> ");
        return scanner.nextLine();
    }

    public String createBackUp(){
        System.out.println("4. Kopia zapasowa");
        System.out.println();
        System.out.print("  [Z]achowaj/[O]dtwórz   : ");
        String decision = scanner.nextLine();
        if(decision.equalsIgnoreCase("Z")){
            return decision;
        } else if(decision.equalsIgnoreCase("O")){
            return decision;
        }
        System.out.println("Wrong input");
        return "";
    }

    public String[] compress(){
        String[] decisions = {"", ""};
        System.out.println("=================================================");
        System.out.print("  Kompresja [G]zip/[Z]ip : ");
        decisions[0] = scanner.nextLine();
        System.out.print("  Nazwa pliku            : ");
        decisions[1] = scanner.nextLine();
        System.out.println("=================================================");
        System.out.println("[Enter] – potwierdź\n" +
                           "[Q] – porzuć");
        return decisions;
    }

    public String decompress(){
        String fileName;
        System.out.println("=================================================");
        System.out.print("  Nazwa pliku            : ");
        fileName = scanner.nextLine();
        System.out.println("=================================================");
        System.out.println("[Enter] – potwierdź\n" +
                           "[Q] – porzuć");
        return fileName;

    }

    public void printWorkers(HashMap<String, Pracownik> pracownicy){
        int pozycja = 1;
        for (Pracownik pracownik : pracownicy.values()){
            System.out.println("1. Lista pracowników");
            System.out.println();
            System.out.println(pracownik.toString());
            System.out.println("                               [Pozycja: "+pozycja+"/"+pracownicy.size()+"]");
            System.out.println("[Enter] - następny");
            System.out.println("[Q] - powrót");

            if (!confirm()) break;
            pozycja++;
        }
    }

    public String getPesel(){
        System.out.print("   Podaj pesel                  :   ");
        return scanner.nextLine();
    }

    public Pracownik removePracownik(HashMap<String, Pracownik> pracownicy) {
        System.out.println("3. Usuń pracownika");
        System.out.println();
        String pesel = getPesel();
        Pracownik pracownik = pracownicy.get(pesel);
        if(pracownik != null){
            System.out.println("=========================================");
            System.out.println(pracownik);
            System.out.println("=========================================");
            System.out.println("[Enter] - potwierdź");
            System.out.println("[Q] - porzuć");
        } else {
            System.out.println("Pesel nie istnieje w bazie pracowników");
            try {
                Thread.sleep(3000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        return pracownik;
    }

    public Pracownik addWorker(){
        System.out.println("2. Dodaj pracownika");
        System.out.println();
        System.out.print("   [D]yrektor/[H]handlowiec: ");
        String wybor = scanner.nextLine().toLowerCase();
        Pracownik worker;
        if(wybor.equalsIgnoreCase("D")){
            worker = addDyrektor();
        } else {
            worker = addHandlowiec();
        }
        return worker;
    }

    private Handlowiec addHandlowiec(){
        String string;
        int integer;
        Handlowiec worker = new Handlowiec();

        System.out.println("=================================================");
        do {
            System.out.print("   Pesel                         :  ");
            string = scanner.nextLine();
            if (Pracownik.isPeselCorrect(string) == 1) {
                worker.setPesel(string);
            }
        } while(Pracownik.isPeselCorrect(string) == 0);

        System.out.print("   Imie                          :  ");
        string = scanner.nextLine();
        worker.setImie(string);

        System.out.print("   Nazwisko                      :  ");
        string = scanner.nextLine();
        worker.setNazwisko(string);

        System.out.print("   Stowisko                      :  ");
        string = scanner.nextLine();
        worker.setStowisko(string);

        System.out.print("   Wynagordzenie                 :  ");
        try {
            integer = scanner.nextInt();
        } catch (Exception e) {
            integer = 0;
        }
        scanner.nextLine();
        worker.setWynagrodzenie(integer);

        System.out.print("   Nr. sluzbowy                  :  ");
        string = scanner.nextLine();
        worker.setNumerSluzbowy(string);

        System.out.print("   Prowizja (zł)                 :  ");
        try {
            integer = scanner.nextInt();
        } catch (Exception e) {
            integer = 0;
        }
        scanner.nextLine();
        worker.setProwizja(integer);

        System.out.print("   Limit prowizji/miesiąc (zł)   :  ");
        try {
            integer = scanner.nextInt();
        }
        catch (Exception e) {
            integer = 0;
        }
        scanner.nextLine();
        worker.setLimit(integer);

        System.out.println("=================================================");
        System.out.println("[Enter] - zapisz");
        System.out.println("[Q] - porzuć");
        return worker;
    }

    private Dyrektor addDyrektor(){
        String string;
        int integer;
        Dyrektor worker = new Dyrektor();

        System.out.println("=================================================");
        do {
            System.out.print("   Pesel                         :  ");
            string = scanner.nextLine();
            if (Pracownik.isPeselCorrect(string) == 1) {
                worker.setPesel(string);
            }
        } while(Pracownik.isPeselCorrect(string) == 0);

        System.out.print("   Imie                          :  ");
        string = scanner.nextLine();
        worker.setImie(string);

        System.out.print("   Nazwisko                      :  ");
        string = scanner.nextLine();
        worker.setNazwisko(string);

        System.out.print("   Stowisko                      :  ");
        string = scanner.nextLine();
        worker.setStowisko(string);

        System.out.print("   Wynagordzenie                 :  ");
        try {
            integer = scanner.nextInt();
        } catch (Exception e) {
            integer = 0;
        }
        scanner.nextLine();
        worker.setWynagrodzenie(integer);

        System.out.print("   Nr. sluzbowy                  :  ");
        string = scanner.nextLine();
        worker.setNumerSluzbowy(string);

        System.out.print("   Dodatek (zł)                  :  ");
        try {
            integer = scanner.nextInt();
        } catch (Exception e) {
            integer = 0;
        }
        scanner.nextLine();
        worker.setDodatek(integer);

        System.out.print("   Numer karty służbowej         :  ");
        string = scanner.nextLine();
        worker.setKarta(string);

        System.out.print("   Limit kosztów/miesiąc (zł)    :  ");
        try{
            integer = scanner.nextInt();
        } catch (Exception e) {
            integer = 0;
        }
        scanner.nextLine();
        worker.setLimit(integer);

        System.out.println("=================================================");
        System.out.println("[Enter] - zapisz");
        System.out.println("[Q] - porzuć");
        return worker;
    }

    public boolean confirm(){
        String confirmation = scanner.nextLine();
        clearConsole();
        return !confirmation.equalsIgnoreCase("Q");
    }

    public void clearConsole() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }
}
