package Model;

import java.io.Serializable;

public class Pracownik implements Serializable {
    private String pesel_ = "";
    private String imie_ = "";
    private String nazwisko_ = "";
    private String stanowisko_ = "";
    private int wynagrodzenie_ = 0;
    private String numerSluzbowy_ = "";

    public Pracownik() {}
    public Pracownik(String pesel, String imie, String nazwisko, String stanowisko, int wynagrodzenie, String numerSluzbowy ) {
        this.pesel_ = pesel;
        this.imie_ = imie;
        this.nazwisko_ = nazwisko;
        this.stanowisko_ = stanowisko;
        this.wynagrodzenie_ = wynagrodzenie;
        this.numerSluzbowy_ = numerSluzbowy;
    }

    public static int isPeselCorrect(String pesel) {
        try {
            int controlSum =
                    ( ((int) pesel.charAt(0))
                    + ((int) pesel.charAt(1)) * 3
                    + ((int) pesel.charAt(2)) * 7
                    + ((int) pesel.charAt(3)) * 9
                    + ((int) pesel.charAt(4))
                    + ((int) pesel.charAt(5)) * 3
                    + ((int) pesel.charAt(6)) * 7
                    + ((int) pesel.charAt(7)) * 9
                    + ((int) pesel.charAt(8))
                    + ((int) pesel.charAt(9)) * 3
                    + ((int) pesel.charAt(10))) % 10;

            if (controlSum == 0) return 1;
            else return 0;
        } catch (RuntimeException e){
            return 0;
        }
    }


    public String getPesel() {
        return pesel_;
    }
    public void setPesel(String pesel) {
        this.pesel_ = pesel;
    }
    public String getImie() {
        return imie_;
    }
    public void setImie(String imie) {
        this.imie_ = imie;
    }
    public String getNazwisko() {
        return nazwisko_;
    }
    public void setNazwisko(String nazwisko) {
        this.nazwisko_ = nazwisko;
    }
    public String getStowisko() {
        return stanowisko_;
    }
    public void setStowisko(String stanowisko) {
        this.stanowisko_ = stanowisko;
    }
    public int getWynagrodzenie() {
        return wynagrodzenie_;
    }
    public void setWynagrodzenie(int wynagrodzenie) {
        this.wynagrodzenie_ = wynagrodzenie;
    }
    public String getNumerSluzbowy() {
        return numerSluzbowy_;
    }
    public void setNumerSluzbowy(String numerSluzbowy) {
        this.numerSluzbowy_ = numerSluzbowy;
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("   Pesel                         :  ").append(pesel_).append("\n");
        sb.append("   Imie                          :  ").append(imie_).append("\n");
        sb.append("   Nazwisko                      :  ").append(nazwisko_).append("\n");
        sb.append("   Stowisko                      :  ").append(stanowisko_).append("\n");
        sb.append("   Wynagordzenie                 :  ").append(wynagrodzenie_).append("\n");
        sb.append("   Nr. sluzbowy                  :  ").append(numerSluzbowy_).append("\n");
        return sb.toString();
    }

}


