import { BrowserRouter as Router, Routes, Route } from 'react-router-dom'
import Layout from './components/Layout/Layout'
import Journal from './pages/Journal'
import Charts from './pages/Charts'
import { UnitProvider } from './contexts/UnitContext'

function App() {
  return (
    <UnitProvider>
      <Router>
        <Layout>
          <Routes>
            <Route path="/" element={<Journal />} />
            <Route path="/charts" element={<Charts />} />
          </Routes>
        </Layout>
      </Router>
    </UnitProvider>
  )
}

export default App 