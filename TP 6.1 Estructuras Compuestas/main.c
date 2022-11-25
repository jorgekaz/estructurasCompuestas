#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define archivoRegistro "registroArchivo.dat"
#define archivoAprobado "registroAprobados.dat"

/// ESTRUCTURAS

typedef struct registro{
    int nota;
    int legajo;
    char nombreApe[50];
    char materia[50];
    int idMateria;
}registroArchivo;

// Estructura de Lista
typedef struct alumno{
    int legajo;
    char nombreApe[50];
    int nota;
}stAlumno;

typedef struct nodo{
    stAlumno nota;
    struct nodo* siguiente;
}nodo;

/// Estructura de Arreglo
typedef struct materia{
    int idMateria;
    char materia[50];
}stMateria;

typedef struct celda{
    stMateria materia;
    nodo* listaDeNotas;
}stCelda;

/// PROTOTIPADO
nodo* inicLista(){
    return NULL;
}

nodo* crearNodo(stAlumno alumno){
    nodo* alumnoNuevo = (nodo*)malloc(sizeof(nodo));
    alumnoNuevo->nota = alumno;
    alumnoNuevo->siguiente = NULL;
    return alumnoNuevo;
}

void cargarAlumno(){
    FILE* fileRegistro = fopen(archivoRegistro,"wb");
    printf("\n");
    if(fileRegistro != NULL){
        registroArchivo alumno;
        printf(" INGRESE MATERIA: ");
        fflush(stdin);
        gets(alumno.materia);
        printf(" INGRESE ID: ");
        fflush(stdin);
        scanf("%i",&alumno.idMateria);
        printf(" INGRESE NOMBRE Y APELLIDO: ");
        fflush(stdin);
        gets(alumno.nombreApe);
        printf(" INGRESE LEGAJO: ");
        fflush(stdin);
        scanf("%i",&alumno.legajo);
        printf(" INGRESE NOTA: ");
        fflush(stdin);
        scanf("%i",&alumno.nota);
        fwrite(&alumno,sizeof(registroArchivo),1,fileRegistro);
        fclose(fileRegistro);
    }else{
        printf(" NO SE PUDO ACCEDER AL ARCHIVO\n");
    }
}

void mostrarRegistro(registroArchivo alumno){
    printf("\n ===================================\n");
    printf(" MATERIA: %s\n", strupr(alumno.materia));
    printf(" ID: %i\n", alumno.idMateria);
    printf(" -----------------------------------\n");
    printf(" NOMBRE Y APELLIDO: %s\n", strupr(alumno.nombreApe));
    printf(" LEGAJO: %i\n", alumno.legajo);
    printf(" NOTA: %i\n", alumno.nota);
    printf(" ===================================\n");
}

void mostrarArchivoRegistro(){
    FILE* fileRegistro = fopen(archivoRegistro,"rb");
    if(fileRegistro!=NULL){
        registroArchivo registro;
        while(fread(&registro,sizeof(registroArchivo),1,fileRegistro)>0){
            mostrarRegistro(registro);
        }
        fclose(fileRegistro);
    }else{
        printf(" NO SE PUDO ACCEDER AL ARCHIVO\n");
    }
}

int buscarPosMateria(stCelda adl[],stMateria materia, int validos){
    int rta = -1;
    int i = 0;
    while((i<validos) && (rta==-1)){
        if(strcmpi(adl[i].materia.materia, materia.materia)==0){
            rta = i;
        }
        i++;
    }
    return rta;
}

int agregar(stCelda adl[], stMateria materia, int validos){
    adl[validos].materia = materia;
    adl[validos].listaDeNotas = inicLista();
    validos++;
    return validos;
}

nodo* buscarUltimo(nodo* lista){
    nodo* aux = lista;
    if(aux != NULL){
        while(aux->siguiente != NULL){
            aux = aux->siguiente;
        }
    }
    return aux;
}

nodo* agregarFinal(nodo* lista, nodo* nuevoNodo){
    if(lista == NULL){
        lista = nuevoNodo;
    }else{
        nodo* ultimo = buscarUltimo(lista);
        ultimo->siguiente = nuevoNodo;
    }
    return lista;
}

int alta(stCelda adl[], stMateria materia, stAlumno nota, int validos){
    nodo* alumnoNota = crearNodo(nota);
    int pos = buscarPosMateria(adl,materia,validos);
    if(pos == -1){
        validos = agregar(adl,materia,validos);
        pos = validos-1;
    }
    adl[pos].listaDeNotas = agregarFinal(adl[pos].listaDeNotas, alumnoNota);
    return validos;
}

int pasarDeArchivoToADL(stCelda adl[], int validos, int dimension){
    FILE* fileRegistro = fopen(archivoRegistro,"rb");
    if(fileRegistro!=NULL){
        registroArchivo registro;
        stMateria materia;
        stAlumno alumno;
        while((fread(&registro,sizeof(registroArchivo),1,fileRegistro)>0) && (validos<dimension)){
            materia.idMateria = registro.idMateria;
            strcpy(materia.materia, registro.materia);
            alumno.legajo = registro.legajo;
            strcpy(alumno.nombreApe, registro.nombreApe);
            alumno.nota = registro.nota;
            validos = alta(adl,materia,alumno,validos);
        }
        fclose(fileRegistro);
    }else{
        printf(" NO SE PUDO ACCEDER AL ARCHIVO\n");
    }
    return validos;
}

void mostrarUnNodo(nodo* aux){
    mostrarNota(aux->nota);
}

void mostrarNota(stAlumno nota){
    printf(" NOMBRE Y APELLIDO: %s \n", strupr(nota.nombreApe));
    printf(" LEGAJO: %i \n", nota.legajo);
    printf(" NOTA: %i \n", nota.nota);
    printf(" ----------------------------------------\n");
}

void recorrerYmostrar(nodo* lista){
    nodo* aux = lista;
    while(aux!=NULL){
        mostrarUnNodo(aux);
        printf("\n");
        aux = aux->siguiente;
    }
}

int cuentaAlumnos(nodo* lista){
    return (lista)?1+cuentaAlumnos(lista->siguiente):0;
}

void mostrarADL(stCelda adl[],int validos){
    int i = 0;
    while(i<validos){
        printf("\n MATERIA: %s",strupr(adl[i].materia.materia));
        printf("\n LISTADO ALUMNOS: %d\n",cuentaAlumnos(adl[i].listaDeNotas));
        printf(" ----------------------------------------\n");
        recorrerYmostrar(adl[i].listaDeNotas);
        i++;
    }
}

void guardarRegistroAprobados(char nombreArchivo[], registroArchivo registroAprobado){
    FILE* fileAprobados = fopen(nombreArchivo,"wb");
    if(fileAprobados!=NULL){
        fwrite(&registroAprobado,sizeof(registroArchivo),1,fileAprobados);
        fclose(fileAprobados);
    }
}

void pasarDeADLToArchivoDeAprobados(stCelda adl[], int validos){
    registroArchivo registro;
    for(int i=0; i<validos; i++){
        registro.idMateria = adl[i].materia.idMateria;
        strcpy(registro.materia, adl[i].materia.materia);

        nodo* aux = adl[i].listaDeNotas;
        while(aux!=NULL){
            registro.legajo = aux->nota.legajo;
            registro.nota = aux->nota.nota;
            strcpy(registro.nombreApe,aux->nota.nombreApe);
            if(aux->nota.nota>=6){
                guardarRegistroAprobados(archivoAprobado,registro);
            }
            aux = aux->siguiente;
        }
    }
}

void mostrarArchivoRegistroAprobados(){
    FILE* fileRegistro = fopen(archivoAprobado,"rb");
    if(fileRegistro!=NULL){
        registroArchivo registro;
        while(fread(&registro,sizeof(registroArchivo),1,fileRegistro)>0){
            mostrarRegistro(registro);
        }
        fclose(fileRegistro);
    }else{
        printf(" NO SE PUDO ACCEDER AL ARCHIVO\n");
    }
}

/// MAIN

int main()
{
    stCelda adl[50];
    int validos = 0;
    int dimension = 50;
    //cargarAlumno();
    printf(" ===========================================================\n");
    printf(" REGISTRO DEL ARCHIVO\n");
    mostrarArchivoRegistro();

    validos = pasarDeArchivoToADL(adl,validos,dimension);

    printf(" ===========================================================\n");
    printf(" ARREGLO DE LISTAS\n");
    mostrarADL(adl,validos);

    printf("\n");
    printf(" ===========================================================\n");
    printf(" REGISTRO APROBADOS\n");
    pasarDeADLToArchivoDeAprobados(adl,validos);
    mostrarArchivoRegistroAprobados();

    return 0;
}


/// FUNCIONES
