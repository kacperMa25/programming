package org.example;

import Model.Pracownik;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class PracownikTest {

    @ParameterizedTest
    @ValueSource(strings = {"81050427494",
            "09221454588",
            "50050626141",
            "55041949168",
            "82121278188",
            "65102122555",
            "00280929422",
            "54021234629",
            "73120515651",
            "77051119135"})
    void testValidPesel(String pesel) {
        assertEquals(1, Pracownik.isPeselCorrect(pesel));
    }

    @ParameterizedTest
    @ValueSource(strings = {"12345678901",
            "00000000001",
            "123333333",
            "555555555",
            "888888888",
            "777777777",
            "666666666",
            "333333333",
            "22222433222",
            "444444444"})
    void testInvalidPesel(String pesel) {
        assertEquals(0,Pracownik.isPeselCorrect(pesel));
    }
}
