begin;

COPY klienci (id_klienta, nazwa, numer_telefonu, email, nip) FROM 'dane.in';
end;
