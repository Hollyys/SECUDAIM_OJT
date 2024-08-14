## 240801 [Mall Ware Detecting - Random Forest]

- Test exe file
- Code
    - 선언부
        
        ```python
        import pandas as pd
        from sklearn.model_selection import train_test_split
        from sklearn.ensemble import RandomForestClassifier
        from sklearn.metrics import classification_report, accuracy_score
        from sklearn.preprocessing import StandardScaler
        from sklearn.impute import SimpleImputer
        from sklearn.pipeline import Pipeline
        from sklearn.compose import ColumnTransformer
        from sklearn.preprocessing import OneHotEncoder
        import joblib
        import pefile
        ```
        
    - exe File Parsing
        
        ```python
        def get_entropy(data):
            if len(data) == 0:
                return 0.0
            occurences = array.array('L', [0]*256)
            for x in data:
                occurences[x if isinstance(x, int) else ord(x)] += 1
            entropy = 0
            for x in occurences:
                if x:
                    p_x = float(x) / len(data)
                    entropy -= p_x*math.log(p_x, 2)
            return entropy
        
        def get_resources(pe):
            """Extract resources :
            [entropy, size]"""
            resources = []
            if hasattr(pe, 'DIRECTORY_ENTRY_RESOURCE'):
                try:
                    for resource_type in pe.DIRECTORY_ENTRY_RESOURCE.entries:
                        if hasattr(resource_type, 'directory'):
                            for resource_id in resource_type.directory.entries:
                                if hasattr(resource_id, 'directory'):
                                    for resource_lang in resource_id.directory.entries:
                                        data = pe.get_data(resource_lang.data.struct.OffsetToData, resource_lang.data.struct.Size)
                                        size = resource_lang.data.struct.Size
                                        entropy = get_entropy(data)
        
                                        resources.append([entropy, size])
                except Exception as e:
                    return resources
            return resources
        
        def get_version_info(pe):
            """Return version infos"""
            res = {}
            for fileinfo in pe.FileInfo:
                if fileinfo.Key == 'StringFileInfo':
                    for st in fileinfo.StringTable:
                        for entry in st.entries.items():
                            res[entry[0]] = entry[1]
                if fileinfo.Key == 'VarFileInfo':
                    for var in fileinfo.Var:
                        res[var.entry.items()[0][0]] = var.entry.items()[0][1]
            if hasattr(pe, 'VS_FIXEDFILEINFO'):
                res['flags'] = pe.VS_FIXEDFILEINFO.FileFlags
                res['os'] = pe.VS_FIXEDFILEINFO.FileOS
                res['type'] = pe.VS_FIXEDFILEINFO.FileType
                res['file_version'] = pe.VS_FIXEDFILEINFO.FileVersionLS
                res['product_version'] = pe.VS_FIXEDFILEINFO.ProductVersionLS
                res['signature'] = pe.VS_FIXEDFILEINFO.Signature
                res['struct_version'] = pe.VS_FIXEDFILEINFO.StrucVersion
            return res
        
        def extract_infos(fpath=None, already_pe=None, inference=None):
            res = {}
        
            try:
                if fpath != None:
                    pe = pefile.PE(fpath)
                else:
                    pe = already_pe[1]
            except FileNotFoundError as e:
                print(f"File not found: {fpath}")
            except pefile.PEFormatError as e:
                print(f"PEFormatError: {fpath} does not appear to ba a PE file.")
                print("Error : ", e)
                return
        
            # if fpath!=None and inference==None:
            #     res['md5'] = file_to_md5(fpath)
            # elif already_pe!=None:
            #     res['md5'] = already_pe[0]
        
            res['Machine'] = pe.FILE_HEADER.Machine
            res['SizeOfOptionalHeader'] = pe.FILE_HEADER.SizeOfOptionalHeader
            res['Characteristics'] = pe.FILE_HEADER.Characteristics
            res['MajorLinkerVersion'] = pe.OPTIONAL_HEADER.MajorLinkerVersion
            res['MinorLinkerVersion'] = pe.OPTIONAL_HEADER.MinorLinkerVersion
            res['SizeOfCode'] = pe.OPTIONAL_HEADER.SizeOfCode
            res['SizeOfInitializedData'] = pe.OPTIONAL_HEADER.SizeOfInitializedData
            res['SizeOfUninitializedData'] = pe.OPTIONAL_HEADER.SizeOfUninitializedData
            res['AddressOfEntryPoint'] = pe.OPTIONAL_HEADER.AddressOfEntryPoint
            res['BaseOfCode'] = pe.OPTIONAL_HEADER.BaseOfCode
            try:
                res['BaseOfData'] = pe.OPTIONAL_HEADER.BaseOfData
            except AttributeError:
                res['BaseOfData'] = 0
            res['ImageBase'] = pe.OPTIONAL_HEADER.ImageBase
            res['SectionAlignment'] = pe.OPTIONAL_HEADER.SectionAlignment
            res['FileAlignment'] = pe.OPTIONAL_HEADER.FileAlignment
            res['MajorOperatingSystemVersion'] = pe.OPTIONAL_HEADER.MajorOperatingSystemVersion
            res['MinorOperatingSystemVersion'] = pe.OPTIONAL_HEADER.MinorOperatingSystemVersion
            res['MajorImageVersion'] = pe.OPTIONAL_HEADER.MajorImageVersion
            res['MinorImageVersion'] = pe.OPTIONAL_HEADER.MinorImageVersion
            res['MajorSubsystemVersion'] = pe.OPTIONAL_HEADER.MajorSubsystemVersion
            res['MinorSubsystemVersion'] = pe.OPTIONAL_HEADER.MinorSubsystemVersion
            res['SizeOfImage'] = pe.OPTIONAL_HEADER.SizeOfImage
            res['SizeOfHeaders'] = pe.OPTIONAL_HEADER.SizeOfHeaders
            res['CheckSum'] = pe.OPTIONAL_HEADER.CheckSum
            res['Subsystem'] = pe.OPTIONAL_HEADER.Subsystem
            res['DllCharacteristics'] = pe.OPTIONAL_HEADER.DllCharacteristics
            res['SizeOfStackReserve'] = pe.OPTIONAL_HEADER.SizeOfStackReserve
            res['SizeOfStackCommit'] = pe.OPTIONAL_HEADER.SizeOfStackCommit
            res['SizeOfHeapReserve'] = pe.OPTIONAL_HEADER.SizeOfHeapReserve
            res['SizeOfHeapCommit'] = pe.OPTIONAL_HEADER.SizeOfHeapCommit
            res['LoaderFlags'] = pe.OPTIONAL_HEADER.LoaderFlags
            res['NumberOfRvaAndSizes'] = pe.OPTIONAL_HEADER.NumberOfRvaAndSizes
        
               # Sections
            res['SectionsNb'] = len(pe.sections)
            entropy = list(map(lambda x:x.get_entropy(), pe.sections))
            res['SectionsMeanEntropy'] = sum(entropy)/float(len(entropy))
            res['SectionsMinEntropy'] = min(entropy)
            res['SectionsMaxEntropy'] = max(entropy)
            raw_sizes = list(map(lambda x:x.SizeOfRawData, pe.sections))
            res['SectionsMeanRawsize'] = sum(raw_sizes)/float(len(raw_sizes))
            res['SectionsMinRawsize'] = min(raw_sizes)
            res['SectionsMaxRawsize'] = max(raw_sizes)
            virtual_sizes = list(map(lambda x:x.Misc_VirtualSize, pe.sections))
            res['SectionsMeanVirtualsize'] = sum(virtual_sizes)/float(len(virtual_sizes))
            res['SectionsMinVirtualsize'] = min(virtual_sizes)
            res['SectionMaxVirtualsize'] = max(virtual_sizes)
            
                #Imports
            try:
                res['ImportsNbDLL'] = len(pe.DIRECTORY_ENTRY_IMPORT)
                imports = sum([x.imports for x in pe.DIRECTORY_ENTRY_IMPORT], [])
                res['ImportsNb'] = len(imports)
                res['ImportsNbOrdinal'] = len(list(filter(lambda x:x.name is None, imports)))
            except AttributeError:
                res['ImportsNbDLL'] = 0
                res['ImportsNb'] = 0
                res['ImportsNbOrdinal'] = 0
        
            #Exports
            try:
                res['ExportNb'] = len(pe.DIRECTORY_ENTRY_EXPORT.symbols)
            except AttributeError:
                # No export
                res['ExportNb'] = 0
            #Resources
            resources= get_resources(pe)
            res['ResourcesNb'] = len(resources)
        
            if len(resources)> 0:
                entropy = list(map(lambda x:x[0], resources))
                res['ResourcesMeanEntropy'] = sum(entropy)/float(len(entropy))
                res['ResourcesMinEntropy'] = min(entropy)
                res['ResourcesMaxEntropy'] = max(entropy)
                sizes = list(map(lambda x:x[1], resources))
                res['ResourcesMeanSize'] = sum(sizes)/float(len(sizes))
                res['ResourcesMinSize'] = min(sizes)
                res['ResourcesMaxSize'] = max(sizes)
            else:
                res['ResourcesNb'] = 0
                res['ResourcesMeanEntropy'] = 0
                res['ResourcesMinEntropy'] = 0
                res['ResourcesMaxEntropy'] = 0
                res['ResourcesMeanSize'] = 0
                res['ResourcesMinSize'] = 0
                res['ResourcesMaxSize'] = 0
        
            # Load configuration size
            try:
                res['LoadConfigurationSize'] = pe.DIRECTORY_ENTRY_LOAD_CONFIG.struct.Size
            except AttributeError:
                res['LoadConfigurationSize'] = 0
        
            # Version configuration size
            try:
                version_infos = get_version_info(pe)
                res['VersionInformationSize'] = len(version_infos.keys())
            except AttributeError:
                res['VersionInformationSize'] = 0
            return res
        ```
        
        ```python
        df = extract_infos(fpath='/home/crossrunway/jupy/SECUDAIM_OJT/PYTHON/PEheader/DoublyLinkedListHashTable.exe', already_pe=None, inference=None)
        ```
        
        exe파일 Parsing 결과물
        
        ```bash
        {'Machine': 332,
         'SizeOfOptionalHeader': 224,
         'Characteristics': 263,
         'MajorLinkerVersion': 2,
         'MinorLinkerVersion': 28,
         'SizeOfCode': 12800,
         'SizeOfInitializedData': 19968,
         'SizeOfUninitializedData': 512,
         'AddressOfEntryPoint': 4832,
         'BaseOfCode': 4096,
         'BaseOfData': 20480,
         'ImageBase': 4194304,
         'SectionAlignment': 4096,
         'FileAlignment': 512,
         'MajorOperatingSystemVersion': 4,
         'MinorOperatingSystemVersion': 0,
         'MajorImageVersion': 1,
         'MinorImageVersion': 0,
         'MajorSubsystemVersion': 4,
         'MinorSubsystemVersion': 0,
         'SizeOfImage': 81920,
         'SizeOfHeaders': 1024,
         'CheckSum': 64577,
         'Subsystem': 3,
         'DllCharacteristics': 0,
         'SizeOfStackReserve': 2097152,
         'SizeOfStackCommit': 4096,
         'SizeOfHeapReserve': 1048576,
         'SizeOfHeapCommit': 4096,
         'LoaderFlags': 0,
         'NumberOfRvaAndSizes': 16,
         'SectionsNb': 14,
         'SectionsMeanEntropy': 2.5702329526476606,
         'SectionsMinEntropy': 0.0,
         'SectionsMaxEntropy': 6.075646419262413,
         'SectionsMeanRawsize': 2304.0,
         'SectionsMinRawsize': 0,
         'SectionsMaxRawsize': 12800,
         'SectionsMeanVirtualsize': 1999.2142857142858,
         'SectionsMinVirtualsize': 10,
         'SectionMaxVirtualsize': 12356,
         'ImportsNbDLL': 3,
         'ImportsNb': 54,
         'ImportsNbOrdinal': 0,
         'ExportNb': 0,
         'ResourcesNb': 0,
         'ResourcesMeanEntropy': 0,
         'ResourcesMinEntropy': 0,
         'ResourcesMaxEntropy': 0,
         'ResourcesMeanSize': 0,
         'ResourcesMinSize': 0,
         'ResourcesMaxSize': 0,
         'LoadConfigurationSize': 0,
         'VersionInformationSize': 0}
        ```
        
    - Random Forest 학습부
        
        ```python
        # Load the data
        data = pd.read_csv('/home/crossrunway/jupy/SECUDAIM_OJT/PYTHON/PEheader/preprocessed_data.csv', sep='|')
        
        # Verify data loading
        print("Data loaded successfully, shape:", data.shape)
        
        # Assume 'Label' is the column indicating malware status (1 for malware, 0 for benign)
        X = data.drop('label', axis=1)  # Features
        y = data['label']  # Target
        
        X = X.drop('Name', axis=1)
        X = X.drop('md5', axis=1)
        
        # Preprocess the data
        # Separate numeric and categorical features
        numeric_features = X.select_dtypes(include=['int64', 'float64']).columns
        categorical_features = X.select_dtypes(include=['object']).columns
        
        # Create preprocessing pipelines
        numeric_transformer = Pipeline(steps=[
            ('imputer', SimpleImputer(strategy='mean')),
            ('scaler', StandardScaler())
        ])
        
        categorical_transformer = Pipeline(steps=[
            ('imputer', SimpleImputer(strategy='most_frequent')),
            ('onehot', OneHotEncoder(handle_unknown='ignore'))
        ])
        
        preprocessor = ColumnTransformer(
            transformers=[
                ('num', numeric_transformer, numeric_features),
                ('cat', categorical_transformer, categorical_features)
            ])
        
        # Split the data
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
        
        # Create the model pipeline
        model = Pipeline(steps=[('preprocessor', preprocessor),
                                ('classifier', RandomForestClassifier(n_estimators=100, random_state=42))])
        
        # Train the model
        model.fit(X_train, y_train)
        
        # Evaluate the model
        y_pred = model.predict(X_test)
        
        # Ensure predictions are generated
        print("Predictions made successfully")
        
        print(f"Accuracy: {accuracy_score(y_test, y_pred)}")
        print(classification_report(y_test, y_pred))
        
        # Save the model
        joblib.dump(model, 'random_forest_model.joblib')
        
        # To load and use the model later:
        # loaded_model = joblib.load('random_forest_model.joblib')
        # new_predictions = loaded_model.predict(new_data)
        
        ```
        
        학습 결과 정보출력
        
        ```bash
        Data loaded successfully, shape: (138047, 57)
        Predictions made successfully
        Accuracy: 0.9952553422672945
                      precision    recall  f1-score   support
        
                   0       0.99      0.99      0.99      8360
                   1       1.00      1.00      1.00     19250
        
            accuracy                           1.00     27610
           macro avg       0.99      0.99      0.99     27610
        weighted avg       1.00      1.00      1.00     27610
        
        ['random_forest_model.joblib']
        ```
        
    - 최종 Malware 판단부
        
        ```python
        # Load the saved model
        model = joblib.load('random_forest_model.joblib')
        
        features_df = pd.DataFrame.from_dict([df])
        
        # Predict using the model
        prediction = model.predict(features_df)
        
        # Output the result
        if prediction[0] == 1:
            print("The executable is classified as malware.")
        else:
            print("The executable is classified as benign.")
        ```
        
        ```bash
        The executable is classified as malware.
        ```
        
- Accuracy benchmark
    
    ![Untitled](%E2%93%9F%E2%93%A8%E2%93%A3%E2%93%97%E2%93%9E%E2%93%9D%2061a237b23fae43cab76f127edf4ede1a/Untitled%202.png)
    
- Issue
    
    exe파일 Parsing을 통해 추출한 딕셔너리 데이터를 Data Frame으로 변환하지 않아서 오류 발생
    
    ```bash
    ...
    File ~/jupy/jup_notebook/lib/python3.8/site-packages/pandas/core/internals/construction.py:645, in _extract_index(data)
        642         raise ValueError("Per-column arrays must each be 1-dimensional")
        644 if not indexes and not raw_lengths:
    --> 645     raise ValueError("If using all scalar values, you must pass an index")
        647 if have_series:
        648     index = union_indexes(indexes)
    
    ValueError: If using all scalar values, you must pass an index
    ```
    
    <aside>
    💡 features_df = pd.DataFrame.from_dict([df]) 형식으로 모델 input
    
    </aside>
    

## 240801 [Mall Ware Detecting - Gradient Boost]

- Code
    1. Basic xgboost
        
        ```python
        # Split the data into training and test sets
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
        
        # Initialize and train the XGBoost model
        xgboost = xgb.XGBClassifier(n_estimators=30, eval_metric='logloss', random_state=42)
        xgboost.fit(X_train, y_train)
        
        # Evaluate the model
        y_pred = xgboost.predict(X_test)
        print(f"Accuracy: {accuracy_score(y_test, y_pred)}")
        print(classification_report(y_test, y_pred))
        
        # Save the trained model
        joblib.dump(xgboost, 'xgboost_malware_model.joblib')
        
        # Load the model for prediction
        xgboost = joblib.load('xgboost_malware_model.joblib')
        
        # Extract features from a specific executable file
        features_dict = df
        
        # Convert the dictionary to a DataFrame with a single sample
        features_df = pd.DataFrame([features_dict])
        
        # Predict using the model
        prediction = xgboost.predict(features_df)
        
        # Output the result
        if prediction[0] == 1:
            print("The executable is classified as malware.")
        else:
            print("The executable is classified as benign.")
        ```
        
    2. GridSearchCV 추가
        
        xgboost에는 다양한 parameter값이 있는데 GridSearchCV를 통해 가장 효율적인 parameter를 찾아 모델을 확정할 수 있다.
        
        ```python
        # Split the data into training and test sets
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.4, random_state=42)
        
        param_grid = {
            'n_estimators': [50, 100, 200],
            'max_depth': [3, 4, 5],
            'learning_rate': [0.01, 0.1, 0.2],
            'subsample': [0.8, 0.9, 1.0],
            'colsample_bytree': [0.8, 0.9, 1.0],
            'gamma': [0, 0.1, 0.2]
        }
        
        # Initialize and train the XGBoost model
        xgboost = xgb.XGBClassifier(eval_metric='logloss', random_state=42)
        grid_search = GridSearchCV(estimator=xgboost, param_grid=param_grid, scoring='accuracy', cv=3, verbose=1, n_jobs=-1)
        
        grid_search.fit(X_train, y_train)
        
        print("Best Parameters:", grid_search.best_params_)
        
        best_model = grid_search.best_estimator_
        y_pred = best_model.predict(X_test)
        
        print(f"Accuracy: {accuracy_score(y_test, y_pred)}")
        print(classification_report(y_test, y_pred))
        
        joblib.dump(best_model, 'xgboost_malware_model_optimized.joblib')
        
        # Load the model for prediction
        best_model = joblib.load('xgboost_malware_model.joblib')
        
        # Extract features from a specific executable file
        features_dict = df
        
        # Convert the dictionary to a DataFrame with a single sample
        features_df = pd.DataFrame([features_dict])
        
        # Predict using the model
        prediction = best_model.predict(features_df)
        
        # Output the result
        if prediction[0] == 1:
            print("The executable is classified as malware.")
        else:
            print("The executable is classified as benign.")
        ```
        
    
    GridSearchCV
    
- Accuracy benchmark
    
    ![Untitled](%E2%93%9F%E2%93%A8%E2%93%A3%E2%93%97%E2%93%9E%E2%93%9D%2061a237b23fae43cab76f127edf4ede1a/Untitled%203.png)
    
- 테스트한 실행파일 소스 (ThisIsNotMallware.c)
    
    ```c
    void main(){
        
    }
    // 이게 Malware먄 이 세상 모든 프로그램이 다 악성코드임
    // printf("Hello World\n"); 이거도 악성 바이러스 코드임
    ```
    
- 테스트 결과
    
    ![Untitled](%E2%93%9F%E2%93%A8%E2%93%A3%E2%93%97%E2%93%9E%E2%93%9D%2061a237b23fae43cab76f127edf4ede1a/Untitled%204.png)
    
    메모리 및 시스템 접근이 전혀 없는 소스임에도 불구하고 malware로 판단하였으며 이는 기본 및 GridSearchCV 추가된 버전의  Gradient Boost모델과 Random Forest모델 모두에서 같은 결과를 보여주었다.