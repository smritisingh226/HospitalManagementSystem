#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <cstdlib>

using namespace std;

class Appointment {
public:
    string patientName;
    string doctorName;
    string date;
    string symptoms;
    string medication;

    Appointment(string pName, string dName, string d, string symp, string med)
        : patientName(pName), doctorName(dName), date(d), symptoms(symp), medication(med) {}
};

class Patient {
public:
    string name;
    int age;
    string diagnosis;
    string status;
    vector<Appointment> appointments;

    Patient(string n, int a, string d, string s) : name(n), age(a), diagnosis(d), status(s) {}

    void changeStatus(string newStatus) {
        status = newStatus;
        cout << name << " is now " << newStatus << ".\n";
    }

    void bookAppointment(string doctorName, string date, string symptoms, string medication) {
        appointments.push_back(Appointment(name, doctorName, date, symptoms, medication));
        changeStatus("Appointment Booked");
        cout << "Appointment booked successfully.\n";
    }

    void viewLogs() const {
        cout << "\n===Patient Logs===\n";
        cout << "Logs for: " << name << "\t" << "Age: " << age <<endl;
        cout << "Diagnosis: " << diagnosis << "\tStatus: " << status << endl;

        if (!appointments.empty()) {
            cout << "Appointments:\n";
            for (const auto& appointment : appointments) {
                cout << "Doctor: " << appointment.doctorName << "\tDate: " << appointment.date << endl;
                cout << "Symptoms: " << appointment.symptoms << "\n";
                cout << "Medication: " << appointment.medication << "\n\n";
            }
        }

        cout << endl;
    }

    bool operator==(const Patient& other) const {
        return name == other.name;
    }

    bool operator!=(const Patient& other) const {
        return !(*this == other);
    }
};

class Doctor {
public:
    string name;
    string department;
    string specialization;
    int yearsOfExperience;
    vector<Appointment> appointments;

    Doctor(string n, string dept, string spec, int exp)
        : name(n), department(dept), specialization(spec), yearsOfExperience(exp) {}

    bool isAvailableInDepartment(const string& targetDepartment) const {
        return department == targetDepartment;
    }
};

class Hospital {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    unordered_map<string, vector<Doctor>> departmentDoctors;

public:
    void addOrUpdatePatient() {
        string name, diagnosis;
        int age;
        cout << "\n===Add A Patient===\n";
        cout << "Enter patient name: ";
        cin >> name;

        auto patientIt = find_if(patients.begin(), patients.end(),
                                 [name](const Patient& p) { return p.name == name; });

        if (patientIt != patients.end()) {
            // Update patient details if already present
            cout << "Patient already exists. Updating details.\n";
            
            cout << "\n===Update a Patient===\n";
            
            cout << "Enter patient age: ";
            cin >> age;

            cout << "Enter patient diagnosis: ";
            cin >> diagnosis;

            patientIt->age = age;
            patientIt->diagnosis = diagnosis;
        } else {
            // Add new patient if not present
            cout << "Enter patient age: ";
            cin >> age;

            cout << "Enter patient diagnosis: ";
            cin >> diagnosis;

            patients.push_back(Patient(name, age, diagnosis, "Registered"));
            cout << "Patient added successfully.\n";
        }
    }

    void admitPatient() {
        string patientName, department, date, symptoms, medication;
        int age;
        // Additional details for patient admission
        string diagnosis, insuranceProvider;
        cout << "\n===Admit a Patient===\n";
        cout << "Enter patient name: ";
        cin >> patientName;

        auto patientIt = find_if(patients.begin(), patients.end(),
                                 [patientName](const Patient& p) { return p.name == patientName; });

        if (patientIt != patients.end()) {
            cout << "Enter patient age: ";
            cin >> age;

            cout << "Enter patient diagnosis: ";
            cin >> diagnosis;

            cout << "Enter patient insurance provider: ";
            cin >> insuranceProvider;

            cout << "Enter the department you want to assign the patient to: ";
            cin >> department;

            if (departmentDoctors.find(department) == departmentDoctors.end()) {
                cout << "Invalid department. No doctors available in the specified department.\n";
                return;
            }

            // Assign a random case doctor from the specified department
            if (departmentDoctors[department].empty()) {
                cout << "No doctors available in the " << department << " department.\n";
                return;
            }

            size_t randomIndex = rand() % departmentDoctors[department].size();
            Doctor caseDoctor = departmentDoctors[department][randomIndex];

            cout << "Patient assigned to Dr. " << caseDoctor.name << " in the " << department << " department.\n";

            patientIt->changeStatus("Admitted");

            patients.push_back(Patient(patientName, age, diagnosis, "Admitted"));  // New patients are admitted by default

            cout << "Enter admission date: ";
            cin >> date;

            cout << "Enter symptoms exhibited: ";
            cin.ignore();  // Ignore newline character left in the stream
            getline(cin, symptoms);

            cout << "Enter prescribed medication (if any): ";
            getline(cin, medication);

            caseDoctor.appointments.push_back(Appointment(patientName, caseDoctor.name, date, symptoms, medication));
            cout << "Patient admitted successfully.\n";
        } else {
            cout << "Patient not found.\n";
        }
    }

    void dischargePatient() {
        string patientName;
        
        cout << "\n===Discharge A Patient===\n";

        cout << "Enter the name of the patient to discharge: ";
        cin >> patientName;

        auto patientIt = find_if(patients.begin(), patients.end(),
                                 [patientName](const Patient& p) { return p.name == patientName; });

        if (patientIt != patients.end()) {
            patientIt->changeStatus("Discharged");
        } else {
            cout << "Patient not found.\n";
        }
    }

    void viewPatients() const {
        cout << "\n===View A Patient===\n";
        if (patients.empty()) {
            cout << "No patients in the system.\n";
        } else {
            cout << "Enter patient name to view logs: ";
            string patientName;
            cin >> patientName;

            auto patientIt = find_if(patients.begin(), patients.end(),
                                     [patientName](const Patient& p) { return p.name == patientName; });

            if (patientIt != patients.end()) {
                patientIt->viewLogs();
            } else {
                cout << "Patient not found.\n";
            }
        }
    }

    void addDoctor() {
        string name, department, specialization;
        int yearsOfExperience;
        
        cout << "\n===Add A Doctor===\n";

        cout << "Enter doctor name: ";
        cin >> name;

        cout << "Enter doctor department: ";
        cin >> department;

        cout << "Enter doctor specialization: ";
        cin >> specialization;

        cout << "Enter years of experience: ";
        cin >> yearsOfExperience;

        Doctor doctor(name, department, specialization, yearsOfExperience);
        doctors.push_back(doctor);
        departmentDoctors[department].push_back(doctor);

        cout << "Doctor added successfully.\n";
    }

    void viewDoctors() const {
        
        cout << "\n===View Doctors and Departments===\n";
        if (departmentDoctors.empty()) {
            cout << "No doctors in the system.\n";
        } else {
            cout << "Departments and Doctors:\n";
            for (const auto& entry : departmentDoctors) {
                cout << "Department: " << entry.first << "\n";
                for (const auto& doctor : entry.second) {
                    cout << "\tName: " << doctor.name << "\tSpecialization: " << doctor.specialization
                         << "\tExperience: " << doctor.yearsOfExperience << " years\n";
                }
                cout << endl;
            }
        }
    }

    void bookAppointment() {
        string patientName, department, date, symptoms, medication;
        
        cout << "\n===Book An Appointment===\n";

        cout << "Enter patient name: ";
        cin >> patientName;

        auto patientIt = find_if(patients.begin(), patients.end(),
                                 [patientName](const Patient& p) { return p.name == patientName; });

        if (patientIt != patients.end()) {
            cout << "Enter the department you want to book an appointment in: ";
            cin >> department;

            if (departmentDoctors.find(department) == departmentDoctors.end()) {
                cout << "Invalid department. No doctors available in the specified department.\n";
                return;
            }

            cout << "Doctors available in the " << department << " department:\n";
            for (const auto& doctor : departmentDoctors[department]) {
                cout << "Name: " << doctor.name << "\t\tSpecialization: " << doctor.specialization
                     << "\tExperience: " << doctor.yearsOfExperience << "\t years\n";
            }

            cout << "Enter the name of the doctor you want to book an appointment with: ";
            string chosenDoctor;
            cin >> chosenDoctor;

            auto doctorIt = find_if(departmentDoctors[department].begin(), departmentDoctors[department].end(),
                                    [chosenDoctor](const Doctor& d) { return d.name == chosenDoctor; });

            if (doctorIt != departmentDoctors[department].end()) {
                cout << "Enter appointment date: ";
                cin >> date;

                cout << "Enter symptoms: ";
                cin.ignore();  // Ignore newline character left in the stream
                getline(cin, symptoms);

                cout << "Enter prescribed medication: ";
                getline(cin, medication);

                doctorIt->appointments.push_back(Appointment(patientName, chosenDoctor, date, symptoms, medication));
                patientIt->bookAppointment(chosenDoctor, date, symptoms, medication);
            } else {
                cout << "Invalid doctor name. Appointment booking failed.\n";
            }
        } else {
            cout << "Patient not found. Appointment booking failed.\n";
        }
    }

    void viewAppointmentsForPatient(const string& patientName) const {
        auto patientIt = find_if(patients.begin(), patients.end(),
                                 [patientName](const Patient& p) { return p.name == patientName; });

        if (patientIt != patients.end()) {
            patientIt->viewLogs();
        } else {
            cout << "Patient not found.\n";
        }
    }
};

int main() {
    Hospital hospital;
    int choice;

    do {
        cout << "\nHospital Management System\n";
        cout << "1. Add or Update Patient\n";
        cout << "2. View Patients\n";
        cout << "3. Add Doctor\n";
        cout << "4. View Departments and Doctors\n";
        cout << "5. Admit Patient\n";
        cout << "6. Discharge Patient\n";
        cout << "7. Book Appointment\n";
        cout << "8. View Appointments for a Patient\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                hospital.addOrUpdatePatient();
                break;
            case 2:
                hospital.viewPatients();
                break;
            case 3:
                hospital.addDoctor();
                break;
            case 4:
                hospital.viewDoctors();
                break;
            case 5:
                hospital.admitPatient();
                break;
            case 6:
                hospital.dischargePatient();
                break;
            case 7:
                hospital.bookAppointment();
                break;
            case 8:
                {
                    string patientName;
                    cout << "Enter patient name: ";
                    cin >> patientName;
                    hospital.viewAppointmentsForPatient(patientName);
                }
                break;
            case 9:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 9);

    return 0;
}
