DROP TABLE kliadba;
DROP TABLE iluzia;
DROP TABLE typ_el_kuz;
DROP TABLE typ_el_grim;
DROP TABLE syn_el_kuz;
DROP TABLE magicke_miesto;
DROP TABLE kuzelny_svitok;
DROP TABLE kuzelnik_vlastnil_grimoar;
DROP TABLE grimoar;
DROP TABLE element;
DROP TABLE kuzelnik;
DROP TABLE kuzlo;

DROP SEQUENCE id_vlastnictvo_seq;
DROP SEQUENCE kuzlo_seq;
DROP SEQUENCE magicke_miesto_seq;
DROP SEQUENCE svitok_seq;

DROP FUNCTION is_rc;
DROP PROCEDURE priemerne_presakovanie_elementu;
DROP PROCEDURE clean_up_kuzelnik;

DROP MATERIALIZED VIEW elementy_magickych_miest_v_londyne;

--creating table Kouzelnik
CREATE TABLE kuzelnik (
    rodne_cislo VARCHAR(10) CHECK (LENGTH(TRIM(TRANSLATE(rodne_cislo, '0123456789',' '))) = 0) PRIMARY KEY,
    meno varchar(255) NOT NULL,
    priezvisko varchar(255) NOT NULL,
    velkost_many int DEFAULT 1,
    uroven_kuzlenia int DEFAULT 1,
    zije varchar(1) CHECK (zije = 'Y' or zije = 'N')
);

-- sequence for id_kuzlo
CREATE SEQUENCE kuzlo_seq
    START WITH 100
    INCREMENT BY 1;
--creating table kuzlo
CREATE TABLE kuzlo
(
    id_kuzlo             int DEFAULT kuzlo_seq.NEXTVAL PRIMARY KEY,
    nazov                varchar(255) NOT NULL,
    typ                  varchar(255),
    sila                 int DEFAULT 1,
    zlozitost_zosielania int DEFAULT 1
);

--creating table grimoar
CREATE TABLE grimoar (
    isbn VARCHAR(13) PRIMARY KEY,
    nazov varchar(255) NOT NULL,
    mnozstvo_magie int DEFAULT 1,
    drzi_kuzelnik VARCHAR(10),
    vlastni_kuzelnik VARCHAR(10),
    kuzlo1 INT NOT NULL,
    kuzlo2 INT NOT NULL,
    kuzlo3 INT NOT NULL,
    kuzlo4 INT NOT NULL,
    kuzlo5 INT NOT NULL,
    kuzlo6 INT NOT NULL,
    kuzlo7 INT NOT NULL,
    kuzlo8 INT NOT NULL,
    kuzlo9 INT NOT NULL,
    kuzlo10 INT NOT NULL,
    kuzlo11 INT DEFAULT NULL,
    kuzlo12 INT DEFAULT NULL,
    kuzlo13 INT DEFAULT NULL,
    kuzlo14 INT DEFAULT NULL,
    kuzlo15 INT DEFAULT NULL,
    -- drzi
    CONSTRAINT FK_drzi_kuzelnik FOREIGN KEY (drzi_kuzelnik)
        REFERENCES kuzelnik(rodne_cislo),
    -- vlastni
    CONSTRAINT FK_vlastni_kuzelnik FOREIGN KEY (vlastni_kuzelnik)
        REFERENCES kuzelnik(rodne_cislo),
    -- isbn check
    CONSTRAINT isbn_valid_check CHECK ( LENGTH(TRIM(TRANSLATE(SUBSTR(isbn, 1, 9), '0123456789',' '))) = 0 AND
                                        LENGTH(isbn) = 10 AND SUBSTR(isbn, 10, 1) = 'X' AND
                                        MOD(TO_NUMBER(SUBSTR(isbn, 1, 1)) * 10 + TO_NUMBER(SUBSTR(isbn, 2, 1)) * 9 +
                                            TO_NUMBER(SUBSTR(isbn, 3, 1)) * 8 + TO_NUMBER(SUBSTR(isbn, 4, 1)) * 7 +
                                            TO_NUMBER(SUBSTR(isbn, 5, 1)) * 6 + TO_NUMBER(SUBSTR(isbn, 6, 1)) * 5 +
                                            TO_NUMBER(SUBSTR(isbn, 7, 1)) * 4 + TO_NUMBER(SUBSTR(isbn, 8, 1)) * 3 +
                                            TO_NUMBER(SUBSTR(isbn, 9, 1)) * 2 + 10, 11) = 0
                                        OR LENGTH(TRIM(TRANSLATE(isbn, '0123456789',' '))) = 0 AND
                                        LENGTH(isbn) = 10 AND SUBSTR(isbn, 10, 1) <> 'X' AND
                                        MOD(TO_NUMBER(SUBSTR(isbn, 1, 1)) * 10 + TO_NUMBER(SUBSTR(isbn, 2, 1)) * 9 +
                                            TO_NUMBER(SUBSTR(isbn, 3, 1)) * 8 + TO_NUMBER(SUBSTR(isbn, 4, 1)) * 7 +
                                            TO_NUMBER(SUBSTR(isbn, 5, 1)) * 6 + TO_NUMBER(SUBSTR(isbn, 6, 1)) * 5 +
                                            TO_NUMBER(SUBSTR(isbn, 7, 1)) * 4 + TO_NUMBER(SUBSTR(isbn, 8, 1)) * 3 +
                                            TO_NUMBER(SUBSTR(isbn, 9, 1)) * 2 + TO_NUMBER(SUBSTR(isbn, 10, 1)), 11) = 0
                                        OR LENGTH(TRIM(TRANSLATE(isbn, '0123456789',' '))) = 0 AND
                                        LENGTH(isbn) = 13 AND MOD(TO_NUMBER(SUBSTR(isbn, 1, 1)) +
                                            TO_NUMBER(SUBSTR(isbn, 2, 1)) * 3 + TO_NUMBER(SUBSTR(isbn, 3, 1)) +
                                            TO_NUMBER(SUBSTR(isbn, 4, 1)) * 3 + TO_NUMBER(SUBSTR(isbn, 5, 1)) +
                                            TO_NUMBER(SUBSTR(isbn, 6, 1)) * 3 + TO_NUMBER(SUBSTR(isbn, 7, 1)) +
                                            TO_NUMBER(SUBSTR(isbn, 8, 1)) * 3 + TO_NUMBER(SUBSTR(isbn, 9, 1)) +
                                            TO_NUMBER(SUBSTR(isbn, 10, 1)) * 3 + TO_NUMBER(SUBSTR(isbn, 11, 1)) +
                                            TO_NUMBER(SUBSTR(isbn, 12, 1)) * 3 + TO_NUMBER(SUBSTR(isbn, 13, 1)), 10) = 0
                                        ),
    -- kuzla fk
    CONSTRAINT FK_kuzlo1 FOREIGN KEY (kuzlo1)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT FK_kuzlo2 FOREIGN KEY (kuzlo2)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT FK_kuzlo3 FOREIGN KEY (kuzlo3)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT FK_kuzlo4 FOREIGN KEY (kuzlo4)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT FK_kuzlo5 FOREIGN KEY (kuzlo5)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT FK_kuzlo6 FOREIGN KEY (kuzlo6)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT FK_kuzlo7 FOREIGN KEY (kuzlo7)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT FK_kuzlo8 FOREIGN KEY (kuzlo8)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT FK_kuzlo9 FOREIGN KEY (kuzlo9)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT FK_kuzlo10 FOREIGN KEY (kuzlo10)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT FK_kuzlo11 FOREIGN KEY (kuzlo11)
        REFERENCES kuzlo(id_kuzlo) ON DELETE SET NULL,
    CONSTRAINT FK_kuzlo12 FOREIGN KEY (kuzlo12)
        REFERENCES kuzlo(id_kuzlo) ON DELETE SET NULL,
    CONSTRAINT FK_kuzlo13 FOREIGN KEY (kuzlo13)
        REFERENCES kuzlo(id_kuzlo) ON DELETE SET NULL,
    CONSTRAINT FK_kuzlo14 FOREIGN KEY (kuzlo14)
        REFERENCES kuzlo(id_kuzlo) ON DELETE SET NULL,
    CONSTRAINT FK_kuzlo15 FOREIGN KEY (kuzlo15)
        REFERENCES kuzlo(id_kuzlo) ON DELETE SET NULL
);

--creating table Element
CREATE TABLE element (
    nazov_elementu varchar(255) PRIMARY KEY,
    specializacia varchar(255) NOT NULL,
    farba varchar(255)
);

-- sequence for id_miesto
CREATE SEQUENCE magicke_miesto_seq
    START WITH 100
    INCREMENT BY 1;
--creating table Magicke miesto
CREATE TABLE magicke_miesto (
    id_miesto int DEFAULT NULL PRIMARY KEY ,
    nazov varchar(255) NOT NULL,
    lokacia varchar(255),
    miera_prosakovania int DEFAULT 1,
    prosakuje_element varchar(255) CONSTRAINT prosakuje_element_NN NOT NULL,
    -- prosakuje NOT NULL
    CONSTRAINT FK_prosakuje_element FOREIGN KEY (prosakuje_element)
        REFERENCES element(nazov_elementu) ON DELETE CASCADE
);

-- trigger na automacticke generovanie hodnoty primarneho kluca pre magicke_miesto
CREATE OR REPLACE TRIGGER magicke_miesto_auto_pk
  BEFORE INSERT ON magicke_miesto
  FOR EACH ROW
BEGIN
    :NEW.id_miesto := magicke_miesto_seq.nextval;
END;

-- sequence for id_svitok
CREATE SEQUENCE svitok_seq
    START WITH 100
    INCREMENT BY 1;
--create table kuzelny svitok
CREATE TABLE kuzelny_svitok (
    id_svitok int DEFAULT svitok_seq.NEXTVAL PRIMARY KEY,
    nazov varchar(255) NOT NULL,
    pouzitie TIMESTAMP,
    obsahuje_kuzlo int CONSTRAINT obsahuje_kuzlo_NN NOT NULL,
    --obsahuje
    CONSTRAINT FK_obsahuje_kuzlo FOREIGN KEY (obsahuje_kuzlo)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE
);

--create specializace
CREATE TABLE kliadba (
    id_kuzlo int PRIMARY KEY,
    miera_zla int DEFAULT 1,
    CONSTRAINT FK_id_kliadba FOREIGN KEY (id_kuzlo)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE
);

CREATE TABLE iluzia (
    id_kuzlo int PRIMARY KEY,
    doba_trvania int DEFAULT 1,
    CONSTRAINT FK_id_iluzia FOREIGN KEY (id_kuzlo)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE
);

-----------------------------------------
-- Creating "vazebne entity"
-----------------------------------------
CREATE TABLE typ_el_grim (
    isbn VARCHAR(13) NOT NULL,
    nazov_elementu varchar(255) CONSTRAINT element_ma_el_grim_NN NOT NULL,
    typ varchar(255) NOT NULL,
    CONSTRAINT FK_nazov_elementu_ma_el_grim FOREIGN KEY (nazov_elementu)
        REFERENCES element(nazov_elementu) ON DELETE CASCADE,
    CONSTRAINT FK_isbn_ma_el_grim FOREIGN KEY (isbn)
        REFERENCES grimoar(isbn) ON DELETE CASCADE,
    CONSTRAINT typ_primary_secondary_only_typ_el_grim CHECK (typ = 'primary' OR typ = 'secondary'),
    PRIMARY KEY (isbn, nazov_elementu)
);

CREATE TABLE typ_el_kuz (
    id_kuzlo int NOT NULL,
    nazov_elementu varchar(255) CONSTRAINT element_ma_el_kuz_NN NOT NULL,
    typ varchar(255) NOT NULL,
    CONSTRAINT FK_nazov_elementu_ma_el_kuz FOREIGN KEY (nazov_elementu)
        REFERENCES element(nazov_elementu) ON DELETE CASCADE,
    CONSTRAINT FK_id_kuzlo_ma_el_kuz FOREIGN KEY (id_kuzlo)
        REFERENCES kuzlo(id_kuzlo) ON DELETE CASCADE,
    CONSTRAINT typ_primary_secondary_only_typ_el_kuz CHECK (typ = 'primary' OR typ = 'secondary'),
    PRIMARY KEY (id_kuzlo, nazov_elementu)
);


CREATE SEQUENCE id_vlastnictvo_seq
    START WITH 10
    INCREMENT BY 1;
CREATE TABLE kuzelnik_vlastnil_grimoar (
    id int DEFAULT id_vlastnictvo_seq.NEXTVAL PRIMARY KEY,
    od date NOT NULL,
    do date,
    rodne_cislo VARCHAR(10),
    isbn VARCHAR(13) NOT NULL,
    CONSTRAINT FK_kuzelnik FOREIGN KEY (rodne_cislo)
        REFERENCES kuzelnik(rodne_cislo),
    CONSTRAINT FK_grimoar FOREIGN KEY (isbn)
        REFERENCES grimoar(isbn) ON DELETE CASCADE
);

CREATE TABLE syn_el_kuz(
    rodne_cislo VARCHAR(10),
    nazov_elementu VARCHAR(255),
    CONSTRAINT FK_syn_kuzelnik FOREIGN KEY (rodne_cislo)
        REFERENCES kuzelnik(rodne_cislo) ON DELETE CASCADE,
    CONSTRAINT FK_syn_element FOREIGN KEY (nazov_elementu)
        REFERENCES element(nazov_elementu) ON DELETE CASCADE,
    PRIMARY KEY (rodne_cislo, nazov_elementu)
);

-- uloha 4
-- funckia pouzita v triggeri
CREATE OR REPLACE FUNCTION is_rc (rc_string IN VARCHAR)
   RETURN INT
IS
   temp_num NUMBER;
BEGIN
    -- 10 miestne musi byt delitelne 11
    IF LENGTH(rc_string) = 10 THEN
        temp_num := TO_NUMBER(SUBSTR(rc_string,1,1)) - TO_NUMBER(SUBSTR(rc_string,2,1)) +
               TO_NUMBER(SUBSTR(rc_string,3,1)) - TO_NUMBER(SUBSTR(rc_string,4,1)) +
               TO_NUMBER(SUBSTR(rc_string,5,1)) - TO_NUMBER(SUBSTR(rc_string,6,1)) +
               TO_NUMBER(SUBSTR(rc_string,7,1)) - TO_NUMBER(SUBSTR(rc_string,8,1)) +
               TO_NUMBER(SUBSTR(rc_string,9,1)) - TO_NUMBER(SUBSTR(rc_string,10,1));
        IF MOD(temp_num, 11) <> 0 THEN
            RETURN 0;
        END IF;
    -- 9 miestne niesmie mat koncovku 000
    ELSIF LENGTH(rc_string) = 9 THEN
        IF SUBSTR(rc_string, -3,3) = '000' THEN
            RETURN 0;
        END IF;
    ELSE
        RETURN 0;
    END IF;
    -- kontrola mesiaca
    temp_num := TO_NUMBER(SUBSTR(rc_string,3,2));
    IF temp_num > 50 THEN
        temp_num := temp_num - 50;
    END IF;
    IF temp_num < 1 OR temp_num > 12 THEN
        RETURN 0;
    END IF;
    -- kontrola dna
    temp_num := TO_NUMBER(SUBSTR(rc_string,5,2));
    IF temp_num > 31 OR temp_num < 1 THEN
        RETURN 0;
    END IF;
    RETURN 1;
END is_rc;

-- trigger na kontrolu rodneho cisla kuzelnikov
CREATE OR REPLACE TRIGGER rodne_cislo_check
    BEFORE
    INSERT OR UPDATE
    ON kuzelnik
    FOR EACH ROW
BEGIN
    IF is_rc(:NEW.rodne_cislo) = 0 THEN
        RAISE_APPLICATION_ERROR(-20100,'rodne_cislo constraint violated.\n');
    END IF;
END;

--creating entry in kuzel_vlastnil_grimoar when grimoar
CREATE OR REPLACE TRIGGER got_grimoar
    AFTER INSERT OR UPDATE ON grimoar
    FOR EACH ROW
    WHEN (NEW.vlastni_kuzelnik IS NOT NULL)
BEGIN
    INSERT INTO kuzelnik_vlastnil_grimoar (id, od, do, rodne_cislo, isbn)
    VALUES (DEFAULT, CURRENT_DATE, NULL, :NEW.vlastni_kuzelnik, :NEW.isbn);
end;

-- vyplnenie datamy
-- elementy
INSERT INTO element VALUES ('water', 'healing', 'blue');
INSERT INTO element VALUES ('ice', 'defense', 'lightblue');
INSERT INTO element VALUES ('fire', 'ofense', 'red');
INSERT INTO element VALUES ('air', 'utility', 'lightgrey');
INSERT INTO element VALUES ('ground', 'defense', 'brown');
INSERT INTO element VALUES ('life', 'healing', 'green');
INSERT INTO element VALUES ('blood', 'ofense', 'velvet');
INSERT INTO element VALUES ('necromantic', 'ofense', 'black');
-- magicke miesta
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Deep Well', 'Dark Forest', 5, 'water');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Mount Doom', 'Mordor', 10, 'fire');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Notrh Pole', 'Antartica', 8, 'ice');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Hogwarts', 'United Kingdom', 4, 'air');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Cemetery', 'Hong Kong', 20, 'necromantic');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Cemetery', 'New York', 14, 'necromantic');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Cemetery', 'Mordor', 15, 'necromantic');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Cemetery', 'United Kingdom', 4, 'necromantic');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Cemetery', 'London', 11, 'necromantic');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Cemetery', 'Hong Kong', 2, 'necromantic');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Cemetery', 'Hong Kong', 10, 'necromantic');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Cemetery', 'Hong Kong', 3, 'necromantic');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Smith', 'London', 4, 'fire');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Bakery', 'London', 1, 'fire');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Volcano', 'Tokyo', 81, 'fire');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Volcano', 'Honolulu', 62, 'fire');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Deep water', 'Antarctica', 42, 'ice');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Refrigerator', 'London', 8, 'ice');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Refrigerator', 'Hong Kong', 9, 'ice');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Beach rock', 'Honolulu', 12, 'water');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Sakura', 'Tokyo', 36, 'water');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Golden Well', 'United Kingdom', 69, 'water');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Fountain', 'Tokyo', 4, 'water');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Gonb', 'Hros bouse', 420, 'water');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Toilet', 'London', 5, 'water');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Calton Bridge', 'Hong Kong', 3, 'air');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Baloon', 'London', 1, 'air');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Cemetery', 'London', 10, 'air');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Cemetery', 'London', 37, 'fire');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Disneyland', 'Paris', 29, 'life');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Big Tree', 'Dark Forest', 29, 'life');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'House of Mary', 'Magic Island', 19, 'life');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Svethol', 'London', 8, 'blood');
-- kuzelnik
INSERT INTO kuzelnik VALUES ('9759126399', 'Tom', 'Riddle', 1000, 10, 'Y');
INSERT INTO kuzelnik VALUES ('9303227197', 'Harry', 'Potter', 999, 10, 'Y');
INSERT INTO kuzelnik VALUES ('0162202469', 'Albus', 'Dumbledore', 1000, 10, 'Y');
INSERT INTO kuzelnik VALUES ('9402198872', 'Imio', 'Sakura', 60, 2, 'Y');
INSERT INTO kuzelnik VALUES ('9659049499', 'May', 'Tompson', 10, 1, 'Y');
INSERT INTO kuzelnik VALUES ('0108107857', 'Clare', 'Priton', 257, 3, 'Y');
INSERT INTO kuzelnik VALUES ('9912095941', 'Dear', 'Liz', 463, 3, 'Y');
INSERT INTO kuzelnik VALUES ('0003107225', 'Michael', 'Crompton', 921, 7, 'Y');
INSERT INTO kuzelnik VALUES ('9854277323', 'Harry', 'Sitlen', 221, 4, 'Y');
INSERT INTO kuzelnik VALUES ('0008173968', 'Tusodo', 'Moejiku', 364, 5, 'Y');
INSERT INTO kuzelnik VALUES ('9555138076', 'Raten', 'Poleoaki', 764, 5, 'N');
INSERT INTO kuzelnik VALUES ('9354147099', 'Hamil', 'Reatiru', 1600, 2, 'Y');
INSERT INTO kuzelnik VALUES ('9852268580', 'Amanda', 'Nurtilo', 300, 2, 'Y');
INSERT INTO kuzelnik VALUES ('9859176822', 'Michal', 'Koval', 1600, 12, 'Y');
INSERT INTO kuzelnik VALUES ('9259138933', 'Norbert', 'Pocs', 1705, 11, 'Y');
-- syn_el_kuz
INSERT INTO syn_el_kuz VALUES ('9759126399','fire');
INSERT INTO syn_el_kuz VALUES ('9759126399','blood');
INSERT INTO syn_el_kuz VALUES ('0162202469', 'water');
INSERT INTO syn_el_kuz VALUES ('0162202469', 'air');
INSERT INTO syn_el_kuz VALUES ('0162202469', 'ice');
INSERT INTO syn_el_kuz VALUES ('9402198872', 'ground');
INSERT INTO syn_el_kuz VALUES ('9402198872', 'water');
INSERT INTO syn_el_kuz VALUES ('9659049499', 'water');
INSERT INTO syn_el_kuz VALUES ('9659049499', 'ice');
INSERT INTO syn_el_kuz VALUES ('9659049499', 'air');
INSERT INTO syn_el_kuz VALUES ('0108107857', 'air');
INSERT INTO syn_el_kuz VALUES ('0108107857', 'water');
INSERT INTO syn_el_kuz VALUES ('0108107857', 'life');
INSERT INTO syn_el_kuz VALUES ('9912095941', 'necromantic');
INSERT INTO syn_el_kuz VALUES ('9912095941', 'blood');
INSERT INTO syn_el_kuz VALUES ('9912095941', 'fire');
INSERT INTO syn_el_kuz VALUES ('0003107225', 'ground');
INSERT INTO syn_el_kuz VALUES ('9854277323', 'ground');
INSERT INTO syn_el_kuz VALUES ('9854277323', 'water');
INSERT INTO syn_el_kuz VALUES ('0008173968', 'air');
INSERT INTO syn_el_kuz VALUES ('0008173968', 'fire');
INSERT INTO syn_el_kuz VALUES ('9555138076', 'blood');
INSERT INTO syn_el_kuz VALUES ('9555138076', 'water');
INSERT INTO syn_el_kuz VALUES ('9555138076', 'ice');
INSERT INTO syn_el_kuz VALUES ('9354147099', 'ice');
INSERT INTO syn_el_kuz VALUES ('9354147099', 'ground');
INSERT INTO syn_el_kuz VALUES ('9354147099', 'water');
INSERT INTO syn_el_kuz VALUES ('9354147099', 'necromantic');
INSERT INTO syn_el_kuz VALUES ('9852268580', 'blood');
INSERT INTO syn_el_kuz VALUES ('9852268580', 'life');
INSERT INTO syn_el_kuz VALUES ('9852268580', 'fire');
INSERT INTO syn_el_kuz VALUES ('9852268580', 'air');
INSERT INTO syn_el_kuz VALUES ('9859176822', 'life');
INSERT INTO syn_el_kuz VALUES ('9859176822', 'blood');
INSERT INTO syn_el_kuz VALUES ('9859176822', 'necromantic');
INSERT INTO syn_el_kuz VALUES ('9859176822', 'air');
INSERT INTO syn_el_kuz VALUES ('9859176822', 'water');
INSERT INTO syn_el_kuz VALUES ('9859176822', 'ice');
INSERT INTO syn_el_kuz VALUES ('9259138933', 'fire');
INSERT INTO syn_el_kuz VALUES ('9259138933', 'blood');
INSERT INTO syn_el_kuz VALUES ('9259138933', 'life');
INSERT INTO syn_el_kuz VALUES ('9259138933', 'ground');
INSERT INTO syn_el_kuz VALUES ('9259138933', 'water');
INSERT INTO syn_el_kuz VALUES ('9259138933', 'ice');
-- kuzla
INSERT INTO kuzlo VALUES (1, 'Invisibility spell', 'utility', '5', '7');
INSERT INTO iluzia VALUES (1, 60);
INSERT INTO typ_el_kuz VALUES (1, 'air', 'primary');
INSERT INTO kuzlo VALUES (2, 'Create ilusion', 'utility', '4', '5');
INSERT INTO iluzia VALUES (2, 300);
INSERT INTO typ_el_kuz VALUES (2, 'air', 'primary');
INSERT INTO kuzlo VALUES (3, 'Avada Kedavra', 'ofense', '9', '10');
INSERT INTO kliadba VALUES (3, 100);
INSERT INTO typ_el_kuz VALUES (3, 'life', 'primary');
INSERT INTO typ_el_kuz VALUES (3, 'fire', 'secondary');
INSERT INTO kuzlo VALUES (4, 'Curse', 'ofense', '4', '6');
INSERT INTO kliadba VALUES (4, 30);
INSERT INTO typ_el_kuz VALUES (4, 'ground', 'primary');
INSERT INTO kuzlo VALUES (5, 'Crucio', 'ofense', '6', '8');
INSERT INTO kliadba VALUES (5, 90);
INSERT INTO typ_el_kuz VALUES (4, 'life', 'primary');
INSERT INTO kuzlo VALUES (6, 'Fiendfyre', 'ofense', '9', '9');
INSERT INTO kliadba VALUES (6, 80);
INSERT INTO typ_el_kuz VALUES (6, 'fire', 'primary');
INSERT INTO kuzlo VALUES (7, 'Imperio', 'utility', '8', '10');
INSERT INTO kliadba VALUES (7, 70);
INSERT INTO iluzia VALUES (7, 600);
INSERT INTO typ_el_kuz VALUES (7, 'life', 'primary');
INSERT INTO kuzlo VALUES (8, 'Fireball', 'ofense', '6', '6');
INSERT INTO typ_el_kuz VALUES (8, 'fire', 'primary');
INSERT INTO kuzlo VALUES (9, 'Ice barrage', 'crowd control', '8', '9');
INSERT INTO typ_el_kuz VALUES (9, 'ice', 'primary');
INSERT INTO typ_el_kuz VALUES (9, 'water', 'secondary');
INSERT INTO kuzlo VALUES (10, 'Life drain', 'healing', '6', '9');
INSERT INTO kliadba VALUES (10, 50);
INSERT INTO typ_el_kuz VALUES (10, 'blood', 'primary');
INSERT INTO typ_el_kuz VALUES (10, 'water', 'secondary');
INSERT INTO kuzlo VALUES (11, 'Water bubble', 'defense', '3', '2');
INSERT INTO typ_el_kuz VALUES (11, 'water', 'primary');
INSERT INTO kuzlo VALUES (12, 'Stone skin', 'defense', '6', '5');
INSERT INTO typ_el_kuz VALUES (12, 'ground', 'primary');
INSERT INTO kuzlo VALUES (13, 'Create image', 'utility', '5', '7');
INSERT INTO typ_el_kuz VALUES (13, 'air', 'primary');
INSERT INTO iluzia VALUES (13, 1200);
INSERT INTO kuzlo VALUES (14, 'Cure wound', 'utility', '5', '7');
INSERT INTO typ_el_kuz VALUES (14, 'life', 'primary');
INSERT INTO typ_el_kuz VALUES (14, 'water', 'secondary');
INSERT INTO kuzlo VALUES (15, 'Deflect spell', 'defense', '7', '7');
INSERT INTO typ_el_kuz VALUES (15, 'air', 'primary');
-- kuzelny svitok
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Healing scroll', CURRENT_TIMESTAMP, 14);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Healing scroll', CURRENT_TIMESTAMP, 14);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Healing scroll', NULL, 14);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Healing scroll', NULL, 14);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Healing scroll', NULL, 14);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Fire scroll', CURRENT_TIMESTAMP, 8);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Fire scroll', NULL, 8);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Fire scroll', NULL, 8);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Fire scroll', NULL, 8);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Invisibility scroll', NULL, 1);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Invisibility scroll', CURRENT_TIMESTAMP, 1);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Invisibility scroll', CURRENT_TIMESTAMP, 1);
INSERT INTO kuzelny_svitok VALUES (DEFAULT, 'Invisibility scroll', NULL, 1);
-- grimoar
INSERT INTO grimoar
VALUES ('8020401059', 'Codex Umbra', 3000, '9759126399', '9759126399', 1, 10, 14, 12, 11, 13, 9, 7, 3, 5, 8, 6, NULL, NULL, NULL);
INSERT INTO  typ_el_grim VALUES ('8020401059', 'fire', 'primary');
INSERT INTO  typ_el_grim VALUES ('8020401059', 'blood', 'secondary');
INSERT INTO kuzelnik_vlastnil_grimoar VALUES (DEFAULT, TO_DATE('2003/07/09', 'yyyy/mm/dd'), TO_DATE('2014/1/20', 'yyyy/mm/dd'), '0162202469', '8020401059');
INSERT INTO grimoar
VALUES ('9788072038848', 'Book of Spells', 1000, '9402198872', '9402198872', 1, 2, 8, 9, 10, 11, 12, 13, 14, 15, NULL, NULL, NULL, NULL, NULL);
INSERT INTO  typ_el_grim VALUES ('9788072038848', 'water', 'primary');
INSERT INTO  typ_el_grim VALUES ('9788072038848', 'air', 'secondary');

INSERT INTO grimoar
VALUES ('034963694X', 'Necromonicon', 1000, '0162202469', '9759126399', 1, 2, 8, 9, 7, 11, 12, 5, 4, 15, 3, NULL, NULL, NULL, NULL);
INSERT INTO  typ_el_grim VALUES ('034963694X', 'ground', 'primary');
INSERT INTO  typ_el_grim VALUES ('034963694X', 'blood', 'secondary');
INSERT INTO grimoar
VALUES ('0349644322', 'Life tips', 2000, '9402198872', '9402198872', 2,3,5,6,7,12,11,14,15, 1, NULL, NULL, NULL, NULL, NULL);
INSERT INTO  typ_el_grim VALUES ('0349644322', 'life', 'primary');
INSERT INTO  typ_el_grim VALUES ('0349644322', 'water', 'secondary');
INSERT INTO grimoar
VALUES ('0349653941', 'Mage handbook', 500, '9912095941', '9402198872', 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, NULL, NULL, NULL, NULL, NULL);
INSERT INTO  typ_el_grim VALUES ('0349653941', 'fire', 'primary');
INSERT INTO  typ_el_grim VALUES ('0349653941', 'air', 'secondary');
INSERT INTO grimoar
VALUES ('9780935696011', 'Warlock 101', 2100, '9912095941', '9555138076', 1, 2, 8, 9, 7, 11, 12, 5, 4, 15, 3, NULL, NULL, NULL, NULL);
INSERT INTO  typ_el_grim VALUES ('9780935696011', 'necromantic', 'primary');
INSERT INTO  typ_el_grim VALUES ('9780935696011', 'fire', 'secondary');
INSERT INTO grimoar
VALUES ('9780786915514', 'Minion magic', 700, '9555138076', '9555138076', 3, 5, 7, 9, 11, 12, 13, 15, 2, 1, 8, 6, 10, NULL, NULL);
INSERT INTO  typ_el_grim VALUES ('9780786915514', 'blood', 'primary');
INSERT INTO  typ_el_grim VALUES ('9780786915514', 'air', 'secondary');
INSERT INTO grimoar
VALUES ('9780786966110', 'Air nomads', 8000, '9859176822', '9259138933', 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
INSERT INTO  typ_el_grim VALUES ('9780786966110', 'air', 'primary');
INSERT INTO  typ_el_grim VALUES ('9780786966110', 'life', 'secondary');
INSERT INTO grimoar
VALUES ('9780786966011', 'Players Handbook', 10000, NULL, NULL, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
INSERT INTO  typ_el_grim VALUES ('9780786966011', 'life', 'primary');
INSERT INTO  typ_el_grim VALUES ('9780786966011', 'blood', 'secondary');
INSERT INTO grimoar
VALUES ('9780786966592', 'How to handle Voldemort', 9800, '9303227197', '9303227197', 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
INSERT INTO  typ_el_grim VALUES ('9780786966592', 'life', 'primary');
INSERT INTO  typ_el_grim VALUES ('9780786966592', 'blood', 'secondary');

-- 4. uloha
-- procedura - vypise priemerne prosakovanie elementu
-- odchytava chyby delenia nulou
CREATE OR REPLACE PROCEDURE priemerne_presakovanie_elementu(nazov_el element.nazov_elementu%TYPE) AS
    CURSOR kurzor IS SELECT mm.miera_prosakovania FROM magicke_miesto mm WHERE mm.prosakuje_element = nazov_el;
    prosak magicke_miesto.miera_prosakovania%TYPE;
    prosak_spolu magicke_miesto.miera_prosakovania%TYPE;
    prosak_priemer magicke_miesto.miera_prosakovania%TYPE;
    najdenich_miest NUMBER;
    BEGIN
        najdenich_miest := 0;
        prosak_spolu := 0;
        najdenich_miest := 0;
        OPEN kurzor;
        LOOP
            FETCH kurzor INTO prosak;
            EXIT WHEN kurzor%NOTFOUND;
            prosak_spolu := prosak_spolu + prosak;
            najdenich_miest := najdenich_miest + 1;
        END loop;
        CLOSE kurzor;
        prosak_priemer := prosak_spolu/najdenich_miest;
        DBMS_OUTPUT.PUT_LINE('Priemerne presakovanie elementu ' || nazov_el || ' na celom svete: ' || prosak_priemer);
    EXCEPTION WHEN ZERO_DIVIDE THEN
        BEGIN
            DBMS_OUTPUT.PUT_LINE('Element ' || nazov_el || ' vobec nepresakuje');
        END;
    END;
    /
-- demonstracia
CALL priemerne_presakovanie_elementu('fire');
CALL priemerne_presakovanie_elementu('air');

-- demonstracia triggeru na kontrolu rodnych cisel
-- nespravne rodne cisla
INSERT INTO kuzelnik VALUES ('1234', 'Example', 'Person', 1000, 10, 'Y');
INSERT INTO kuzelnik VALUES ('000123', 'Example', 'Person', 1000, 10, 'Y');
INSERT INTO kuzelnik VALUES ('9759126398', 'Example', 'Person', 1000, 10, 'Y');
-- spravne rodne cisla
INSERT INTO kuzelnik VALUES ('9309195962', 'Proper', 'Person', 1000, 10, 'Y'); -- 10 miestne
INSERT INTO kuzelnik VALUES ('480919596', 'Person', 'Proper', 1000, 10, 'Y'); -- 9 miestne

-- demonstracia triggeru na generovanie primarneho kluca pre magicke_miesto
INSERT INTO magicke_miesto VALUES (NULL, 'Test1', 'Volakde', 10, 'fire');
INSERT INTO magicke_miesto VALUES (DEFAULT, 'Test2', 'Uz Inde', 12, 'ice'); -- DEFAULT hodnota je NULL takze sa trigger aktivuje
SELECT * FROM magicke_miesto;

--Pri umrti kuzelnika sa poupratuje po nom aj v ostatnych tabulkach
CREATE OR REPLACE PROCEDURE clean_up_kuzelnik (rc_kuzelnik varchar2) IS
BEGIN
    UPDATE kuzelnik_vlastnil_grimoar
        SET kuzelnik_vlastnil_grimoar.do = CURRENT_DATE
        WHERE kuzelnik_vlastnil_grimoar.rodne_cislo = clean_up_kuzelnik.rc_kuzelnik;

    UPDATE grimoar
        SET grimoar.vlastni_kuzelnik = NULL
        WHERE grimoar.vlastni_kuzelnik = clean_up_kuzelnik.rc_kuzelnik;

    UPDATE grimoar
        SET grimoar.drzi_kuzelnik = NULL
        WHERE grimoar.drzi_kuzelnik = clean_up_kuzelnik.rc_kuzelnik;
end;

--trigger na umrtie kuzelnika
CREATE OR REPLACE TRIGGER kuzelnik_died
    BEFORE UPDATE OR INSERT ON kuzelnik
    FOR EACH ROW
    WHEN (NEW.zije = 'N')
BEGIN
    clean_up_kuzelnik(:NEW.rodne_cislo);
end;

--demonstracia umrtia kuzelnika
select * from grimoar where drzi_kuzelnik = 9759126399;
select * from grimoar where vlastni_kuzelnik = 9759126399;
select * from kuzelnik_vlastnil_grimoar;

UPDATE kuzelnik SET zije='N' WHERE rodne_cislo = 9759126399;

select * from grimoar where drzi_kuzelnik = 9759126399;
select * from grimoar where vlastni_kuzelnik = 9759126399;
select * from kuzelnik_vlastnil_grimoar;

-- explain plan pred indexom
-- Ktory je najsilenjsi element?
EXPLAIN PLAN FOR
    SELECT tek.nazov_elementu, sum(sila)
    FROM kuzlo JOIN typ_el_kuz tek ON kuzlo.id_kuzlo = tek.id_kuzlo
    WHERE tek.typ='primary'
    GROUP BY tek.nazov_elementu
    ORDER BY sum(sila);
SELECT PLAN_TABLE_OUTPUT FROM table (DBMS_XPLAN.DISPLAY());

-- index
CREATE INDEX typ_tek_idx
ON typ_el_kuz (typ);

-- explain plan po indexe
-- Ktory je najsilenjsi element?
-- insie
EXPLAIN PLAN FOR
    SELECT tek.nazov_elementu, sum(sila)
    FROM kuzlo JOIN typ_el_kuz tek ON kuzlo.id_kuzlo = tek.id_kuzlo
    WHERE tek.typ='primary'
    GROUP BY tek.nazov_elementu
    ORDER BY sum(sila);
SELECT PLAN_TABLE_OUTPUT FROM table (DBMS_XPLAN.DISPLAY());

-- definicie pristupovych prav
GRANT ALL PRIVILEGES ON element TO xpocsn00;
GRANT ALL PRIVILEGES ON grimoar TO xpocsn00;
GRANT ALL PRIVILEGES ON iluzia TO xpocsn00;
GRANT ALL PRIVILEGES ON kliadba TO xpocsn00;
GRANT ALL PRIVILEGES ON kuzelnik TO xpocsn00;
GRANT ALL PRIVILEGES ON kuzelnik_vlastnil_grimoar TO xpocsn00;
GRANT ALL PRIVILEGES ON kuzelny_svitok TO xpocsn00;
GRANT ALL PRIVILEGES ON kuzlo TO xpocsn00;
GRANT ALL PRIVILEGES ON magicke_miesto TO xpocsn00;
GRANT ALL PRIVILEGES ON syn_el_kuz TO xpocsn00;
GRANT ALL PRIVILEGES ON typ_el_grim TO xpocsn00;
GRANT ALL PRIVILEGES ON typ_el_kuz TO xpocsn00;
GRANT ALL PRIVILEGES ON id_vlastnictvo_seq TO xpocsn00;
GRANT ALL PRIVILEGES ON kuzlo_seq TO xpocsn00;
GRANT ALL PRIVILEGES ON magicke_miesto_seq TO xpocsn00;
GRANT ALL PRIVILEGES ON svitok_seq TO xpocsn00;
GRANT ALL PRIVILEGES ON is_rc TO xpocsn00;
GRANT ALL PRIVILEGES ON priemerne_presakovanie_elementu TO xpocsn00;
GRANT ALL PRIVILEGES ON clean_up_kuzelnik TO xpocsn00;

-- materialized view
CREATE MATERIALIZED VIEW elementy_magickych_miest_v_londyne
CACHE
BUILD IMMEDIATE
REFRESH ON COMMIT
ENABLE QUERY REWRITE AS
  -- S akymi elementami existuju magicke miesta v Londine?
    SELECT * FROM xkoval17.element
    WHERE EXISTS(SELECT prosakuje_element FROM xkoval17.magicke_miesto WHERE
    xkoval17.element.nazov_elementu=xkoval17.magicke_miesto.prosakuje_element AND xkoval17.magicke_miesto.lokacia='London');

-- demostracia:
SELECT * FROM elementy_magickych_miest_v_londyne;
INSERT INTO xkoval17.magicke_miesto VALUES (DEFAULT, 'Very Groundy Place', 'London', 123, 'ground');
-- este neaktualizovane
SELECT * FROM elementy_magickych_miest_v_londyne;
COMMIT;
-- aktualizovane
SELECT * FROM elementy_magickych_miest_v_londyne;
