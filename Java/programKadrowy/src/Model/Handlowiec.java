package Model;

public class Handlowiec extends Pracownik {
    private int prowizja_ = 0;
    private int limit_ = 0;

    public Handlowiec() {}
    public Handlowiec(String pesel, String imie, String nazwisko, String stanowisko,
                      int wynagrodzenie, String numerSluzbowy, int prowizja,
                      int limit) {
        super(pesel, imie, nazwisko, stanowisko, wynagrodzenie, numerSluzbowy);
        prowizja_ = prowizja;
        limit_ = limit;
    }

    public int getProwizja() { return prowizja_; }
    public void setProwizja(int prowizja) { prowizja_ = prowizja; }
    public int getLimit() { return limit_; }
    public void setLimit(int limit) { limit_ = limit; }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder(super.toString());
        sb.append("   Prowizja (zł)                 :  ").append(prowizja_).append("\n");
        sb.append("   Limit prowizji/miesiąc (zł)   :  ").append(limit_).append("\n");
        return sb.toString();
    }
}
