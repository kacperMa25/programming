package Model;

public class Dyrektor extends Pracownik {
    int dodatek_ = 0;
    String karta_ = "";
    int limit_ = 0;

    public Dyrektor() {}
    public Dyrektor(String pesel, String imie, String nazwisko, String stanowisko, int wynagrodzenie, String numerSluzbowy, int dodatek, String karta, int limit) {
        super(pesel, imie, nazwisko, stanowisko, wynagrodzenie, numerSluzbowy);
        dodatek_ = dodatek;
        karta_ = karta;
        limit_ = limit;
    }


    public int getDodatek() { return dodatek_; }
    public void setDodatek(int dodatek) { dodatek_ = dodatek; }
    public String getKarta() { return karta_; }
    public void setKarta(String karta) { karta_ = karta; }
    public int getLimit() { return limit_; }
    public void setLimit(int limit) { limit_ = limit; }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder(super.toString());
        sb.append("   Dodatek (zł)                  :  ").append(dodatek_).append("\n");
        sb.append("   Numer karty służbowej         :  ").append(karta_).append("\n");
        sb.append("   Limit kosztów/miesiąc (zł)    :  ").append(limit_).append("\n");
        return sb.toString();
    }

}
